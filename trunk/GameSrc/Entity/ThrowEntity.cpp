#include "ThrowEntity.hpp"
#include <Thor/Math.hpp>
#include "../Helper/Conversion.hpp"
#include "../Physics/PhysicsManager.hpp"
#include "../System/GraphicsManager.hpp"
#include "../System/ResourceCache.hpp"
#include "../Event/ContactEventData.hpp"
#include "../Event/SolveEventData.hpp"

REGISTER_ENTITY( ThrowEntity, "Throw")

ThrowEntity::ThrowEntity() : BaseClass(), m_throwBody(*this), m_throwTranslator(*this), m_throwSprite()
{
}

ThrowEntity::~ThrowEntity()
{
	sb::GraphicsManager::getInstance()->removeDrawable(m_throwSprite,2);
	sb::PhysicsManager::getInstance()->removeSimulatable(&m_throwBody);
}

void ThrowEntity::initializeEntity( const TiXmlElement *propertyElement )
{
    BaseClass::initializeEntity(propertyElement);

	const float THROW_SIZE = 2.0f;

    {
		thor::ResourceKey<sf::Texture> key = thor::Resources::fromFile<sf::Texture>("Resource/Ogmo/Entities/Throw.png");
		std::shared_ptr<sf::Texture> texture = sb::ResourceCache::getInstance()->acquire<sf::Texture>(key);
		m_throwSprite.setTexture(*texture);
		m_throwSprite.setOrigin(32,32);

		sb::GraphicsManager::getInstance()->addDrawable(m_throwSprite,2);
    }

    {
        b2BodyDef bodyDefinition;
        bodyDefinition.userData = (Entity*)this;
        bodyDefinition.position = ToVector(getPosition());
        bodyDefinition.angle = getRotation() * DEGTORAD * -1.f;
        bodyDefinition.type = b2_staticBody;

        b2Body* throwBody = sb::PhysicsManager::getInstance()->getPhysicsWorld()->CreateBody(&bodyDefinition);

        b2PolygonShape boxShape;
        boxShape.SetAsBox( 0.5f*THROW_SIZE, 0.5f*THROW_SIZE );

        b2FixtureDef fixtureDefinition;
        fixtureDefinition.shape = &boxShape;
        fixtureDefinition.density = 1.0f;
        fixtureDefinition.friction = 0.0f;
        fixtureDefinition.restitution = 0.5f;

        throwBody->CreateFixture(&fixtureDefinition);

		m_throwBody.setBody(throwBody);

		sb::PhysicsManager::getInstance()->addSimulatable(&m_throwBody);
    }
}

void ThrowEntity::update( sf::Time deltaTime )
{
	BaseClass::update(deltaTime);

	m_throwTranslator.translate(m_throwSprite);
}
