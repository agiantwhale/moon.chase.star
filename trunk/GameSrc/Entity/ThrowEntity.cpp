#include <glog\logging.h>
#include "../Entity/ThrowEntity.hpp"
#include "../System/ResourceManager.hpp"
#include "../Event/ContactEventData.h"

REGISTER_ENTITY( ThrowEntity, "Throw")

const float THROW_SIZE = 2.0f;

ThrowEntity::ThrowEntity() : BaseClass(), _throwBody(this), _throwSprite(this),
                             _targetEntity(NULL)
{
}

ThrowEntity::~ThrowEntity()
{
}

void ThrowEntity::Initialize( const TiXmlElement *propertyElement )
{
    BaseClass::Initialize(propertyElement);

    {
        TextureManager* textureMgr = TextureManager::GetInstance();
		sf::Sprite *throwSprite = new sf::Sprite(*textureMgr->GetResource("Resource/Ogmo/Entities/Throw.png"));
        throwSprite->setOrigin(sf::Vector2f(0.5f*THROW_SIZE*RATIO,0.5f*THROW_SIZE*RATIO));
		_throwSprite.SetSprite( throwSprite );
		_throwSprite.RegisterRenderable( 2 );
    }

    {
        b2BodyDef bodyDefinition;
        bodyDefinition.userData = (IPhysics*)this;
        bodyDefinition.position = b2Vec2(GetPosition().x, GetPosition().y);
		bodyDefinition.angle = GetRotation();
        bodyDefinition.type = b2_staticBody;

		_throwBody.CreateBody( bodyDefinition );

        b2PolygonShape boxShape;
        boxShape.SetAsBox( 0.5f*THROW_SIZE, 0.5f*THROW_SIZE );

        b2FixtureDef fixtureDefinition;
        fixtureDefinition.shape = &boxShape;
        fixtureDefinition.density = 1.0f;
        fixtureDefinition.friction = 0.5f;
        fixtureDefinition.restitution = 0.0f;

		_throwBody.CreateFixture( fixtureDefinition, "Throw" );

		_throwBody.ResetTransform();
	}
}

bool ThrowEntity::HandleEvent(const EventData& theevent)
{
	switch (theevent.GetEventType())
	{
	case Event_BeginContact:
		{
			const ContactEventData& contactData = static_cast<const ContactEventData&>(theevent);
			const b2Contact* contactInfo = contactData.GetContact();

			if(contactInfo->GetFixtureA()==_throwBody.LookUpFixture(("Throw")))
			{
				ProcessContact(contactInfo,contactInfo->GetFixtureB());
			}

			if(contactInfo->GetFixtureB()==_throwBody.LookUpFixture(("Throw")))
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

void ThrowEntity::Simulate(void)
{
    const float THROW_VELOCITY = 40.0f;

    if(_targetEntity)
    {
		b2Vec2 unit = _throwBody.GetBody()->GetWorldVector(b2Vec2(0,1.0f));
        unit *= THROW_VELOCITY;
        _targetEntity->Throw(unit);

        _targetEntity = NULL;
    }
}

void ThrowEntity::ProcessContact(const b2Contact* contact, const b2Fixture* contactFixture )
{
    b2WorldManifold manifold;
    contact->GetWorldManifold(&manifold);

    if( manifold.points[0].y > GetPosition().y )
    {
        b2Vec2 normal = manifold.normal;
        float slope = std::abs(normal.y/normal.x);

        if( slope > 1.0f ) //45 degrees is acceptable.
        {
            IPhysics *interfaceA = GetPhysicsInterface(contact->GetFixtureA());
            IPhysics *interfaceB = GetPhysicsInterface(contact->GetFixtureB());

            if(interfaceA && interfaceA->GetEntity()->GetEntityType() == 'BALL')
            {
				Entity* entity = interfaceA->GetEntity();
				if( entity->GetEntityType() == PlayerEntity::kEntity_PlayerEntity )
				{
					_targetEntity = static_cast<PlayerEntity*>(entity);
				}
            }
            if(interfaceB && interfaceB->GetEntity()->GetEntityType() == 'BALL')
            {
				Entity* entity = interfaceB->GetEntity();
				if( entity->GetEntityType() == PlayerEntity::kEntity_PlayerEntity )
				{
					_targetEntity = static_cast<PlayerEntity*>(entity);
				}
            }
        }
    }
}