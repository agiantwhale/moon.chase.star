#include "TeleportEntity.hpp"
#include "../Helper/Conversion.hpp"
#include "../Event/ContactEventData.hpp"
#include "../Physics/PhysicsManager.hpp"
#include "../System/ResourceCache.hpp"
#include "../System/GraphicsManager.hpp"

REGISTER_ENTITY(TeleportEntity,"Teleport")

const float ROTATION_PER_SECOND = 90.0f;
const float TELEPORT_SIZE = 2.0f;

TeleportEntity::TeleportEntity()
	:	BaseClass(),
		m_entranceTransform(),
		m_entranceTranslator(m_entranceTransform),
		m_triggerBody(*this),
		m_enterSprite()
{
}

TeleportEntity::~TeleportEntity()
{
	sb::GraphicsManager::getInstance()->removeDrawable(m_enterSprite,2);
	sb::PhysicsManager::getInstance()->removeSimulatable(&m_triggerBody);
}

void TeleportEntity::update( sf::Time deltaTime )
{
	m_entranceTransform.setRotation(m_entranceTransform.getRotation() - ROTATION_PER_SECOND * deltaTime.asSeconds());
	m_entranceTranslator.translate(m_enterSprite);
}

void TeleportEntity::initializeEntity( const TiXmlElement *propertyElement /*= NULL */ )
{
	BaseClass::initializeEntity(propertyElement);

	{
		thor::ResourceKey<sf::Texture> key = thor::Resources::fromFile<sf::Texture>("Resource/Ogmo/Entities/Entrance.png");
		std::shared_ptr<sf::Texture> texture = sb::ResourceCache::getInstance()->acquire<sf::Texture>(key);
		m_enterSprite.setTexture(*texture);
		//m_enterSprite.setOrigin(0.5f*TELEPORT_SIZE*RATIO, 0.5*TELEPORT_SIZE*RATIO);
		m_enterSprite.setOrigin(32,28.5);
		sb::GraphicsManager::getInstance()->addDrawable(m_enterSprite,2);
	}

	if(propertyElement)
	{
		{
			m_entranceTransform.setPosition(getPosition());
		}

		{
			m_exitEntity = new TeleportExitEntity;

			const TiXmlElement* exitNode = propertyElement->FirstChildElement("node");
			if(exitNode)
			{
				float x = 0.f, y = 0.f;
				exitNode->QueryFloatAttribute("x",&x);
				exitNode->QueryFloatAttribute("y",&y);

				sf::Vector2f world((x - SCREENWIDTH/2) * UNRATIO, (y - SCREENHEIGHT/2) * UNRATIO * -1);
				m_exitEntity->setPosition(world);
			}

			m_exitEntity->initializeEntity();
			
			sb::EntityManager::getInstance()->registerEntity(m_exitEntity);
		}

		{
			b2BodyDef bodyDefinition;
			bodyDefinition.userData = (Entity*)this;
			bodyDefinition.position = ToVector(getPosition());
			bodyDefinition.angle = 0.0f;
			bodyDefinition.fixedRotation = true;
			bodyDefinition.type = b2_staticBody;

			b2Body* teleportBody = sb::PhysicsManager::getInstance()->getPhysicsWorld()->CreateBody(&bodyDefinition);

			b2PolygonShape boxShape;
			boxShape.SetAsBox( 0.5f*TELEPORT_SIZE, 0.5f*TELEPORT_SIZE );

			b2FixtureDef fixtureDefinition;
			fixtureDefinition.shape = &boxShape;
			fixtureDefinition.isSensor = true;

			teleportBody->CreateFixture(&fixtureDefinition);

			m_triggerBody.setBody(teleportBody);
			sb::PhysicsManager::getInstance()->addSimulatable(&m_triggerBody);
		}
	}
}

TeleportExitEntity::TeleportExitEntity()
	:	BaseClass(),
		m_exitTransform(),
		m_exitTranslator(m_exitTransform),
		m_triggerBody(*this),
		m_exitSprite()
{
}

TeleportExitEntity::~TeleportExitEntity()
{
	sb::GraphicsManager::getInstance()->removeDrawable(m_exitSprite,2);
	sb::PhysicsManager::getInstance()->removeSimulatable(&m_triggerBody);
}


void TeleportExitEntity::update( sf::Time deltaTime )
{
	m_exitTransform.setRotation(m_exitTransform.getRotation() + ROTATION_PER_SECOND * deltaTime.asSeconds());
	m_exitTranslator.translate(m_exitSprite);
}

void TeleportExitEntity::initializeEntity( const TiXmlElement *propertyElement /*= NULL */ )
{
	BaseClass::initializeEntity(propertyElement);

	{
		m_exitTransform.setPosition(getPosition());
	}

	{
		thor::ResourceKey<sf::Texture> key = thor::Resources::fromFile<sf::Texture>("Resource/Ogmo/Entities/Exit.png");
		std::shared_ptr<sf::Texture> texture = sb::ResourceCache::getInstance()->acquire<sf::Texture>(key);
		m_exitSprite.setTexture(*texture);
		//m_exitSprite.setOrigin(0.5f*TELEPORT_SIZE*RATIO, 0.5*TELEPORT_SIZE*RATIO);
		m_exitSprite.setOrigin(32,31);
		sb::GraphicsManager::getInstance()->addDrawable(m_exitSprite,2);
	}

	{
		b2BodyDef bodyDefinition;
		bodyDefinition.userData = (Entity*)this;
		bodyDefinition.position = ToVector(getPosition());
		bodyDefinition.angle = 0.0f;
		bodyDefinition.fixedRotation = true;
		bodyDefinition.type = b2_staticBody;

		b2Body* teleportBody = sb::PhysicsManager::getInstance()->getPhysicsWorld()->CreateBody(&bodyDefinition);

		b2PolygonShape boxShape;
		boxShape.SetAsBox( 0.5f*TELEPORT_SIZE, 0.5f*TELEPORT_SIZE );

		b2FixtureDef fixtureDefinition;
		fixtureDefinition.shape = &boxShape;
		fixtureDefinition.isSensor = true;

		teleportBody->CreateFixture(&fixtureDefinition);

		m_triggerBody.setBody(teleportBody);
		sb::PhysicsManager::getInstance()->addSimulatable(&m_triggerBody);
	}
}
