#include "SharpEntity.hpp"
#include "../System/GraphicsManager.hpp"
#include "../System/ResourceCache.hpp"
#include "../Physics/PhysicsManager.hpp"

REGISTER_ENTITY(SharpEntity, "Sharp")

SharpEntity::SharpEntity()
	:	BaseClass(),
		m_sharpSprite(),
		m_sharpTranslator(*this),
		m_sharpBody(*this)
{
}

SharpEntity::~SharpEntity()
{
	sb::GraphicsManager::getInstance()->removeDrawable(m_sharpSprite,2);
	sb::PhysicsManager::getInstance()->removeSimulatable(&m_sharpBody);
}

void SharpEntity::initializeEntity( const TiXmlElement *propertyElement /*= NULL */ )
{
	BaseClass::initializeEntity(propertyElement);

	{
		thor::ResourceKey<sf::Texture> key = thor::Resources::fromFile<sf::Texture>("Resource/Ogmo/Entities/Sharp.png");
		std::shared_ptr<sf::Texture> texture = sb::ResourceCache::getInstance()->acquire<sf::Texture>(key);
		m_sharpSprite.setTexture(*texture);
		m_sharpSprite.setOrigin(sf::Vector2f(32,60));

		sb::GraphicsManager::getInstance()->addDrawable(m_sharpSprite,2);
	}

	{
		b2BodyDef bodyDefinition;
		bodyDefinition.userData = (Entity*)this;
		bodyDefinition.position = b2Vec2(getPosition().x, getPosition().y);
		bodyDefinition.angle = getRotation() * DEGTORAD * -1.f;
		bodyDefinition.type = b2_staticBody;
		bodyDefinition.gravityScale = 9.0f;

		b2Body* blockBody = sb::PhysicsManager::getInstance()->getPhysicsWorld()->CreateBody(&bodyDefinition);

		b2Vec2 triangle[3];
		
		triangle[0] = b2Vec2(0.f,60.f * UNRATIO);
		triangle[1] = b2Vec2(-32.f * UNRATIO, 0.f );
		triangle[2] = b2Vec2(32.f * UNRATIO, 0.f );

		b2PolygonShape trianleShape;
		trianleShape.Set(triangle,3);
		//boxShape.SetAsBox( 0.5f*BLOCK_SIZE, 0.5f*BLOCK_SIZE );

		b2FixtureDef fixtureDefinition;
		fixtureDefinition.shape = &trianleShape;
		fixtureDefinition.density = 1.0f;
		fixtureDefinition.friction = 0.0f;
		fixtureDefinition.restitution = 0.0f;

		blockBody->CreateFixture(&fixtureDefinition);

		m_sharpBody.setBody(blockBody);

		sb::PhysicsManager::getInstance()->addSimulatable(&m_sharpBody);
	}
}

void SharpEntity::update( sf::Time deltaTime )
{
	BaseClass::update(deltaTime);

	m_sharpTranslator.translate(m_sharpSprite);
}
