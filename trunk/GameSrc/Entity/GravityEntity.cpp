#include "GravityEntity.hpp"
#include "PlayerEntity.hpp"
#include <Thor/Vectors.hpp>
#include "../Helper/Conversion.hpp"
#include "../Physics/PhysicsManager.hpp"
#include "../System/GraphicsManager.hpp"
#include "../System/ResourceCache.hpp"
#include "../System/SceneManager.hpp"
#include "../Event/ContactEventData.hpp"
#include "../Event/GravityChangeEventData.hpp"

REGISTER_ENTITY(GravityEntity,"Gravity")

GravityEntity::GravityEntity()	:
	BaseClass(),
	m_gravityBody(*this),
	m_gravityTranslator(*this),
	m_gravitySprite(),
	m_gravitySound(),
	m_changeGravity(false),
	m_gravityDirection(sb::PhysicsManager::Gravity_Up)
{
	addEventListenType(Event_BeginContact);
	addEventListenType(Event_Simulate);
}

GravityEntity::~GravityEntity()
{
	removeEventListenType(Event_BeginContact);
	removeEventListenType(Event_Simulate);

	sb::GraphicsManager::getInstance()->removeDrawable(m_gravitySprite,2);
	sb::PhysicsManager::getInstance()->removeSimulatable(&m_gravityBody);
}

void GravityEntity::update( sf::Time deltaTime )
{
	BaseClass::update(deltaTime);

	m_gravityTranslator.translate(m_gravitySprite);
}

void GravityEntity::initializeEntity( const TiXmlElement *propertyElement /*= NULL */ )
{
	BaseClass::initializeEntity(propertyElement);

	sf::Vector2f finalGravity = thor::rotatedVector<float>(sf::Vector2f(0,GRAVITY_ACCELERATION), getRotation() * -1.f );

	//HACKHACK!!
	if(std::abs(finalGravity.x) <= 5.0f)
	{
		finalGravity.x = 0.f;
	}

	if(std::abs(finalGravity.y) <= 5.0f)
	{
		finalGravity.y = 0.f;
	}

	if( finalGravity.x < 0.f )
	{
		m_gravityDirection = sb::PhysicsManager::Gravity_Right;
	}

	if( finalGravity.x > 0.f )
	{
		m_gravityDirection = sb::PhysicsManager::Gravity_Left;
	}

	if( finalGravity.y > 0.f )
	{
		m_gravityDirection = sb::PhysicsManager::Gravity_Up;
	}

	if( finalGravity.y < 0.f )
	{
		m_gravityDirection = sb::PhysicsManager::Gravity_Down;
	}


	const float GRAVITY_SIZE = 2.0f;

	{
		thor::ResourceKey<sf::Texture> key = thor::Resources::fromFile<sf::Texture>("Resource/Ogmo/Entities/Gravity.png");
		std::shared_ptr<sf::Texture> texture = sb::ResourceCache::getInstance()->acquire<sf::Texture>(key);
		m_gravitySprite.setTexture(*texture);
		m_gravitySprite.setOrigin(32,32);

		sb::GraphicsManager::getInstance()->addDrawable(m_gravitySprite,2);
	}

	{
		thor::ResourceKey<sf::SoundBuffer> key = thor::Resources::fromFile<sf::SoundBuffer>("Resource/Sounds/gravity.wav");
		std::shared_ptr<sf::SoundBuffer> buffer = sb::ResourceCache::getInstance()->acquire<sf::SoundBuffer>(key);
		m_gravitySound.setBuffer(*buffer);
		m_gravitySound.setVolume(50.f);
	}

	{
		b2BodyDef bodyDefinition;
		bodyDefinition.userData = (Entity*)this;
		bodyDefinition.position = ToVector(getPosition());
		bodyDefinition.angle = getRotation() * DEGTORAD;
		bodyDefinition.type = b2_staticBody;

		b2Body* throwBody = sb::PhysicsManager::getInstance()->getPhysicsWorld()->CreateBody(&bodyDefinition);

		b2PolygonShape boxShape;
		boxShape.SetAsBox( 0.5f*GRAVITY_SIZE, 0.5f*GRAVITY_SIZE );

		b2FixtureDef fixtureDefinition;
		fixtureDefinition.shape = &boxShape;
		fixtureDefinition.density = 1.0f;
		fixtureDefinition.friction = 0.0f;
		fixtureDefinition.restitution = 0.5f;

		throwBody->CreateFixture(&fixtureDefinition);

		m_gravityBody.setBody(throwBody);

		sb::PhysicsManager::getInstance()->addSimulatable(&m_gravityBody);
	}
}

bool GravityEntity::handleEvent( const sb::EventData& theevent )
{
	switch(theevent.getEventType())
	{
	case Event_BeginContact:
		{
			const sb::ContactEventData& contactEventData = static_cast<const sb::ContactEventData&>(theevent);
			const b2Contact* contact = contactEventData.getContact();
			const b2Fixture* fixture = nullptr;

			if(m_gravityBody.checkContact(contact,fixture))
			{
				sb::Entity* entity = sb::getOwnerEntity(fixture);
				PlayerEntity* playerEntity = sb::entity_cast<PlayerEntity>(entity);

				if(playerEntity && m_gravityDirection != sb::PhysicsManager::getInstance()->getGravityDirection() )
				{
					m_changeGravity = true;
					m_gravitySound.play();
				}
			}

			break;
		}

	case Event_Simulate:
		{
			simulate();
			break;
		}

	default:
		{
			break;
		}
	}

	return false;
}

void GravityEntity::simulate()
{
	if(m_changeGravity)
	{
		m_changeGravity = false;

		/*
		sf::Vector2f finalGravity = thor::rotatedVector<float>(sf::Vector2f(0,GRAVITY_ACCELERATION), getRotation() * -1.f );

		//HACKHACK!!
		if(std::abs(finalGravity.x) <= 5.0f)
		{
			finalGravity.x = 0.f;
		}

		if(std::abs(finalGravity.y) <= 5.0f)
		{
			finalGravity.y = 0.f;
		}
		*/

		//sb::PhysicsManager::getInstance()->getPhysicsWorld()->SetGravity(ToVector(finalGravity));

		sb::EventData* eventData = new GravityChangeEventData(m_gravityDirection);
		eventData->triggerEvent();
		sb::SceneManager::getInstance()->getPlayerEntity()->fall();
	}
}



