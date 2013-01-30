#include "../Entity/TeleportEntity.hpp"
#include "../Event/ContactEventData.h"
#include "../System/ResourceCache.hpp"

REGISTER_ENTITY(TeleportEntity,"Teleport")

const float ROTATION_PER_SECOND = 10.0f;
const float TELEPORT_SIZE = 2.0f;

TeleportEntity::TeleportEntity()
	:	BaseClass(),
		_exitTransform(),
		_triggerBody(this),
		_enterSprite(this),
		_exitSprite(&_exitTransform)
{

}

TeleportEntity::~TeleportEntity()
{

}

void TeleportEntity::Update( float deltaTime )
{
	SetRotation(GetRotation() + ROTATION_PER_SECOND * deltaTime);
	_exitTransform.SetRotation(_exitTransform.GetRotation() - ROTATION_PER_SECOND * deltaTime);
}

void TeleportEntity::Initialize( const TiXmlElement *propertyElement /*= NULL */ )
{
	BaseClass::Initialize(propertyElement);

	{
		thor::ResourceKey<sf::Texture> key = thor::Resources::fromFile<sf::Texture>("Resource/Ogmo/Entities/Entrance.png");
		std::shared_ptr<sf::Texture> texture = ResourceCache::GetInstance()->acquire<sf::Texture>(key);
		sf::Sprite* entranceSprite = new sf::Sprite(*texture,sf::IntRect(0,0,32,32));
		entranceSprite->setOrigin(sf::Vector2f(TELEPORT_SIZE*RATIO,TELEPORT_SIZE*RATIO));
		_enterSprite.SetSprite( entranceSprite );
		_enterSprite.RegisterRenderable( 2 );
	}

	{
		thor::ResourceKey<sf::Texture> key = thor::Resources::fromFile<sf::Texture>("Resource/Ogmo/Entities/Exit.png");
		std::shared_ptr<sf::Texture> texture = ResourceCache::GetInstance()->acquire<sf::Texture>(key);
		sf::Sprite* exitSprite = new sf::Sprite(*texture,sf::IntRect(0,0,32,32));
		exitSprite->setOrigin(sf::Vector2f(TELEPORT_SIZE*RATIO,TELEPORT_SIZE*RATIO));
		_exitSprite.SetSprite( exitSprite );
		_exitSprite.RegisterRenderable( 2 );
	}
}
