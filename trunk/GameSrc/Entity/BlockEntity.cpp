#include <SFML/Graphics.hpp>

#include "BlockEntity.hpp"
#include "../System/GraphicsManager.hpp"
#include "../Physics/PhysicsManager.hpp"
#include "../System/ResourceCache.hpp"
#include "../Event/ContactEventData.hpp"

REGISTER_ENTITY(BlockEntity,"Block")

const float BLOCK_SIZE = 2.0f;

BlockEntity::BlockEntity() : BaseClass(), m_shouldFall(false), m_blockSprite(), m_bodyController(*this), m_screenPositioner(*this)
{}

BlockEntity::~BlockEntity()
{
	sb::GraphicsManager::getInstance()->removeDrawable(m_blockSprite,2);
}

void BlockEntity::initializeEntity( const TiXmlElement *propertyElement /* = NULL */ )
{
	BaseClass::initializeEntity(propertyElement);

	{
		thor::ResourceKey<sf::Texture> key = thor::Resources::fromFile<sf::Texture>("Resource/Ogmo/Entities/Block.png");
		std::shared_ptr<sf::Texture> texture = sb::ResourceCache::getInstance()->acquire<sf::Texture>(key);
		m_blockSprite.setTexture(*texture);
		m_blockSprite.setOrigin(sf::Vector2f(0.5f*BLOCK_SIZE*RATIO,0.5f*BLOCK_SIZE*RATIO));

		sb::GraphicsManager::getInstance()->addDrawable(m_blockSprite,2);
	}

	{
		b2BodyDef bodyDefinition;
		bodyDefinition.userData = (Entity*)this;
		bodyDefinition.position = b2Vec2(getPosition().x, getPosition().y);
		bodyDefinition.angle = 0.0f;
		bodyDefinition.type = b2_staticBody;
		bodyDefinition.gravityScale = 9.0f;

		b2Body* blockBody = sb::PhysicsManager::getInstance()->getPhysicsWorld()->CreateBody(&bodyDefinition);

		b2PolygonShape boxShape;
		boxShape.SetAsBox( 0.5f*BLOCK_SIZE, 0.5f*BLOCK_SIZE );

		b2FixtureDef fixtureDefinition;
		fixtureDefinition.shape = &boxShape;
		fixtureDefinition.density = 1.0f;
		fixtureDefinition.friction = 0.5f;
		fixtureDefinition.restitution = 0.0f;

		blockBody->CreateFixture(&fixtureDefinition);

		m_bodyController.setBody(blockBody);
	}
}

bool BlockEntity::handleEvent( const sb::EventData& theevent )
{
	switch(theevent.getEventType())
	{
	case Event_BeginContact:
		{
			const sb::ContactEventData& contactEvent = static_cast<const sb::ContactEventData&>(theevent);

			const b2Fixture* targetFixture = nullptr;
			if( m_bodyController.checkContact(contactEvent.getContact(),targetFixture) )
			{
				processContact(contactEvent.getContact(), targetFixture);
			}
			break;
		}

	case Event_Simulate:
		{
			simulate();
			break;
		}
	}

	return false;
}

void BlockEntity::simulate( void )
{
	if(m_shouldFall)
	{
		m_bodyController.getBody()->SetType(b2_dynamicBody);
	}
}

void BlockEntity::processContact( const b2Contact* contact, const b2Fixture* contactFixture )
{
	Entity* entity = static_cast<Entity*>(contact->GetFixtureA()->GetBody()->GetUserData());
	if( entity->getEntityType() == 'BALL')
	{
		m_shouldFall = true;
	}
}