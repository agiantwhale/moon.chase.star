#include "../Entity/TeleportEntity.hpp"
#include "../Event/ContactEventData.h"
#include "../System/ResourceCache.hpp"

REGISTER_ENTITY(TeleportEntity,"Teleport")

const float ROTATION_PER_SECOND = 90.0f;
const float TELEPORT_SIZE = 2.0f;

TeleportEntity::TeleportEntity()
	:	BaseClass(),
		_exitTransform(),
		_triggerBody(this),
		_enterSprite(&_enterTransform),
		_exitSprite(&_exitTransform)
{
}

TeleportEntity::~TeleportEntity()
{
}

void TeleportEntity::Update( float deltaTime )
{
	_enterTransform.SetRotation(_enterTransform.GetRotation() + ROTATION_PER_SECOND * deltaTime);
	_exitTransform.SetRotation(_exitTransform.GetRotation() - ROTATION_PER_SECOND * deltaTime);
}

void TeleportEntity::Initialize( const TiXmlElement *propertyElement /*= NULL */ )
{
	BaseClass::Initialize(propertyElement);

	{
		thor::ResourceKey<sf::Texture> key = thor::Resources::fromFile<sf::Texture>("Resource/Ogmo/Entities/Entrance.png");
		std::shared_ptr<sf::Texture> texture = ResourceCache::GetInstance()->acquire<sf::Texture>(key);
		sf::Sprite* entranceSprite = new sf::Sprite(*texture);
		entranceSprite->setOrigin(sf::Vector2f(0.5f*TELEPORT_SIZE*RATIO,0.5f*TELEPORT_SIZE*RATIO));
		_enterSprite.SetSprite( entranceSprite );
		_enterSprite.RegisterRenderable( 2 );
	}

	{
		thor::ResourceKey<sf::Texture> key = thor::Resources::fromFile<sf::Texture>("Resource/Ogmo/Entities/Exit.png");
		std::shared_ptr<sf::Texture> texture = ResourceCache::GetInstance()->acquire<sf::Texture>(key);
		sf::Sprite* exitSprite = new sf::Sprite(*texture);
		exitSprite->setOrigin(sf::Vector2f(0.5f*TELEPORT_SIZE*RATIO,0.5f*TELEPORT_SIZE*RATIO));
		_exitSprite.SetSprite( exitSprite );
		_exitSprite.RegisterRenderable( 2 );
	}


	if(propertyElement)
	{
		{
			_enterTransform.SetPosition(GetPosition());
		}


		{
			const TiXmlElement* exitNode = propertyElement->FirstChildElement("node");
			float previousX = GetPosition().x, previousY = GetPosition().y;

			if(exitNode)
			{
				float x = 0.f, y = 0.f;
				exitNode->QueryFloatAttribute("x",&x);
				exitNode->QueryFloatAttribute("y",&y);

				Vec2D world((x - SCREENWIDTH/2) * UNRATIO, (y - SCREENHEIGHT/2) * UNRATIO * -1);
				_exitTransform.SetPosition(world);
			}
		}

		{
			b2BodyDef bodyDefinition;
			bodyDefinition.userData = (IPhysics*)this;
			bodyDefinition.position = b2Vec2(GetPosition().x, GetPosition().y);
			bodyDefinition.angle = 0.0f;
			bodyDefinition.fixedRotation = true;
			bodyDefinition.type = b2_staticBody;

			_triggerBody.CreateBody( bodyDefinition );

			b2PolygonShape boxShape;
			boxShape.SetAsBox( 0.5f*TELEPORT_SIZE, 0.5f*TELEPORT_SIZE );

			b2FixtureDef fixtureDefinition;
			fixtureDefinition.shape = &boxShape;
			fixtureDefinition.isSensor = true;

			_triggerBody.CreateFixture( fixtureDefinition, "Trigger" );

			_triggerBody.ResetTransform();
		}
	}
}
