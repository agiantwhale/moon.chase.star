#include <Thor/Vectors.hpp>
#include "MovingPlatformEntity.hpp"
#include "../Helper/Conversion.hpp"
#include "../System/ResourceCache.hpp"
#include "../System/GraphicsManager.hpp"
#include "../Physics/PhysicsManager.hpp"

REGISTER_ENTITY(MovingPlatformEntity,"MovingPlatform")

const float BLOCK_SIZE = 2.0f;

MovingPlatformEntity::MovingPlatformEntity()
	:	BaseClass(),
		m_platformSprite(),
		m_platformBody(*this),
		m_platformScreenTranslator(*this),
		m_platformRoute(),
		m_platformRouteIterator(),
		m_travelSpeed(0.0f),
		m_moveTimer()
{

}

MovingPlatformEntity::~MovingPlatformEntity()
{
	sb::GraphicsManager::getInstance()->removeDrawable(m_platformSprite,2);
}

void MovingPlatformEntity::update( sf::Time deltaTime )
{
	if(m_moveTimer.isExpired())
	{
		sf::Vector2f currentDestination = *m_platformRouteIterator;
		sf::Vector2f nextDestination = getNextPlatformDestination();
		float travelTime = thor::length<float>(nextDestination - currentDestination) / m_travelSpeed;
		m_moveTimer.restart(sf::seconds(travelTime));
		m_platformBody.getBody()->SetTransform(ToVector(currentDestination),0.f);
		m_platformBody.getBody()->SetAwake(true);
	}

	sf::Vector2f destination = *(m_platformRouteIterator);
	sf::Vector2f velocity = thor::unitVector(destination - getPosition()) * m_travelSpeed;

	m_platformBody.getBody()->SetLinearVelocity(ToVector(velocity));
	m_platformBody.getBody()->SetAwake(true);

	//Updates the platform sprite position to the appropriate screen position
	m_platformScreenTranslator.translate(m_platformSprite);
}

void MovingPlatformEntity::initializeEntity( const TiXmlElement *propertyElement /* = nullptr */ )
{
	BaseClass::initializeEntity(propertyElement);

	{
		thor::ResourceKey<sf::Texture> key = thor::Resources::fromFile<sf::Texture>("Resource/Ogmo/Entities/Block.png");
		std::shared_ptr<sf::Texture> texture = sb::ResourceCache::getInstance()->acquire<sf::Texture>(key);
		m_platformSprite.setTexture(*texture);
		m_platformSprite.setOrigin(sf::Vector2f(BLOCK_SIZE,BLOCK_SIZE) * 0.5f * RATIO);

		sb::GraphicsManager::getInstance()->addDrawable(m_platformSprite,2);
	}

	{
		b2BodyDef bodyDefinition;
		bodyDefinition.userData = (Entity*)this;
		bodyDefinition.position = ToVector(getPosition());
		bodyDefinition.angle = 0.0f;
		bodyDefinition.type = b2_kinematicBody;
		bodyDefinition.gravityScale = 9.0f;
		bodyDefinition.fixedRotation = true;

		b2Body* platformBody = sb::PhysicsManager::getInstance()->getPhysicsWorld()->CreateBody(&bodyDefinition);

		b2PolygonShape boxShape;
		boxShape.SetAsBox( 0.5f*BLOCK_SIZE, 0.5f*BLOCK_SIZE );

		b2FixtureDef fixtureDefinition;
		fixtureDefinition.shape = &boxShape;
		fixtureDefinition.density = 1.0f;
		fixtureDefinition.friction = 0.5f;
		fixtureDefinition.restitution = 0.0f;

		platformBody->CreateFixture(&fixtureDefinition);

		m_platformBody.setBody(platformBody);
	}

	if(propertyElement)
	{
		{
			propertyElement->QueryFloatAttribute("TravelSpeed",&m_travelSpeed);
		}

		{
			float totalDistance = 0.0f;
			m_platformRoute.push_back(getPosition());
			for(const TiXmlElement* pathNode = propertyElement->FirstChildElement("node"); pathNode != nullptr; pathNode = pathNode->NextSiblingElement())
			{
				float x = 0.f, y = 0.f;
				pathNode->QueryFloatAttribute("x",&x);
				pathNode->QueryFloatAttribute("y",&y);

				sf::Vector2f world((x - SCREENWIDTH/2) * UNRATIO, (y - SCREENHEIGHT/2) * UNRATIO * -1);

				totalDistance += thor::length<float>(world - m_platformRoute.back());

				m_platformRoute.push_back(world);
			}
		}

		{
			m_platformRouteIterator = m_platformRoute.begin();
		}
	}
}

const sf::Vector2f& MovingPlatformEntity::getNextPlatformDestination()
{
	m_platformRouteIterator++;

	if(m_platformRouteIterator == m_platformRoute.end())
	{
		m_platformRouteIterator = m_platformRoute.begin();
	}

	return *m_platformRouteIterator;
}
