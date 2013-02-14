#include "../Entity/ThrowEntity.hpp"
#include "../System/ResourceCache.hpp"
#include "../Event/ContactEventData.h"
#include "../Event/SolveEventData.h"

REGISTER_ENTITY( ThrowEntity, "Throw")

ThrowEntity::ThrowEntity() : BaseClass(), _throwBody(this), _throwSprite(this)
{
}

ThrowEntity::~ThrowEntity()
{
}

void ThrowEntity::initializeEntity( const TiXmlElement *propertyElement )
{
    BaseClass::initializeEntity(propertyElement);

	const float THROW_SIZE = 2.0f;

    {
		thor::ResourceKey<sf::Texture> key = thor::Resources::fromFile<sf::Texture>("Resource/Ogmo/Entities/Throw.png");
		std::shared_ptr<sf::Texture> texture = ResourceCache::GetInstance()->acquire<sf::Texture>(key);

		sf::Sprite* throwSprite = new sf::Sprite(*texture);
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
        fixtureDefinition.friction = 0.0f;
        fixtureDefinition.restitution = 0.5f;

        _throwBody.CreateFixture( fixtureDefinition, "Throw" );

        _throwBody.ResetTransform();
    }
}