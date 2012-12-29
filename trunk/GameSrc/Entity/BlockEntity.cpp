#include "../Entity/BlockEntity.hpp"
#include "../System/PhysicsManager.hpp"
#include "../System/ResourceManager.hpp"
#include "../Event/ContactEventData.h"

REGISTER_ENTITY( BlockEntity, "Block")

BlockEntity::BlockEntity() : BaseClass(), _blockBody(this), _blockSprite(this),
    _shouldFall(false)
{
}

BlockEntity::~BlockEntity()
{
}

void BlockEntity::Initialize( const TiXmlElement *propertyElement )
{
    BaseClass::Initialize(propertyElement);

    const float BLOCK_SIZE = 2.0f;

    {
        TextureManager* texturMgr = TextureManager::GetInstance();
        sf::Sprite* blockSprite = new sf::Sprite(*texturMgr->GetResource("Resource/Ogmo/Entities/Block.png"));
        blockSprite->setOrigin(sf::Vector2f(0.5f*BLOCK_SIZE*RATIO,0.5f*BLOCK_SIZE*RATIO));
        _blockSprite.SetSprite( blockSprite );
        _blockSprite.RegisterRenderable( 2 );
    }

    {
        b2BodyDef bodyDefinition;
        bodyDefinition.userData = (IPhysics*)this;
        bodyDefinition.position = b2Vec2(GetPosition().x, GetPosition().y);
        bodyDefinition.angle = 0.0f;
        bodyDefinition.type = b2_staticBody;
        bodyDefinition.gravityScale = 9.0f;

        _blockBody.CreateBody( bodyDefinition );

        b2PolygonShape boxShape;
        boxShape.SetAsBox( 0.5f*BLOCK_SIZE, 0.5f*BLOCK_SIZE );

        b2FixtureDef fixtureDefinition;
        fixtureDefinition.shape = &boxShape;
        fixtureDefinition.density = 1.0f;
        fixtureDefinition.friction = 0.5f;
        fixtureDefinition.restitution = 0.0f;

        _blockBody.CreateFixture( fixtureDefinition, "Block" );

        _blockBody.ResetTransform();
    }
}

bool BlockEntity::HandleEvent(const EventData& theevent)
{
    switch (theevent.GetEventType())
    {
    case Event_BeginContact:
    {
        const ContactEventData& contactData = static_cast<const ContactEventData&>(theevent);
        const b2Contact* contactInfo = contactData.GetContact();

        if(contactInfo->GetFixtureA()==_blockBody.LookUpFixture(("Block")))
        {
            ProcessContact(contactInfo,contactInfo->GetFixtureB());
        }

        if(contactInfo->GetFixtureB()==_blockBody.LookUpFixture(("Block")))
        {
            ProcessContact(contactInfo,contactInfo->GetFixtureA());
        }

        break;
    }

    case Event_Simulate:
    {
        Simulate();
        break;
    }

    default:
        break;
    }

    return false;
}

void BlockEntity::Simulate(void)
{
    if(_shouldFall)
    {
        _blockBody.GetBody()->SetType( b2_dynamicBody );
    }
}

void BlockEntity::ProcessContact( const b2Contact* contact, const b2Fixture* contactFixture )
{
    IPhysics *targetInterface = GetPhysicsInterface(contactFixture);

    if(targetInterface && targetInterface->GetEntity()->GetEntityType() == 'BALL')
    {
        _shouldFall = true;
    }
}