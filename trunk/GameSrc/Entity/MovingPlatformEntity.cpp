#include "../Entity/MovingPlatformEntity.hpp"
#include "../System/ResourceCache.hpp"

REGISTER_ENTITY(MovingPlatformEntity,"MovingPlatform")

const float BLOCK_SIZE = 2.0f;

MovingPlatformEntity::MovingPlatformEntity()
	:	BaseClass(),
		_platformSprite(this),
		_platformBody(this),
		_platformRoute(),
		_platformRouteIterator(),
		_travelSpeed(0.0f),
		_totalTravelTime(0.0f),
		_currentTravelTime(0.0f)
{

}

MovingPlatformEntity::~MovingPlatformEntity()
{

}

void MovingPlatformEntity::Update( float deltaTime )
{
	if(_currentTravelTime <= 0.0f)
	{
		Vec2D currentDestination = *_platformRouteIterator;
		Vec2D nextDestination = GetNextPlatformDestination();
		_currentTravelTime = Magnitude(nextDestination - currentDestination) / _travelSpeed;
		_platformBody.GetBody()->SetTransform(currentDestination,_platformBody.GetBody()->GetAngle());
		_platformBody.GetBody()->SetAwake(true);
	}

	_currentTravelTime -= deltaTime;
	Vec2D destination = *(_platformRouteIterator);
	Vec2D velocity = destination - GetPosition();
	velocity.Normalize();
	velocity *= _travelSpeed;

	_platformBody.GetBody()->SetLinearVelocity(velocity);
	_platformBody.GetBody()->SetAwake(true);
}

void MovingPlatformEntity::Initialize( const TiXmlElement *propertyElement /* = nullptr */ )
{
	BaseClass::Initialize(propertyElement);

	{
		thor::ResourceKey<sf::Texture> key = thor::Resources::fromFile<sf::Texture>("Resource/Ogmo/Entities/Block.png");
		std::shared_ptr<sf::Texture> texture = ResourceCache::GetInstance()->acquire<sf::Texture>(key);
		sf::Sprite* blockSprite = new sf::Sprite(*texture);
		blockSprite->setOrigin(sf::Vector2f(0.5f*BLOCK_SIZE*RATIO,0.5f*BLOCK_SIZE*RATIO));
		_platformSprite.SetSprite( blockSprite );
		_platformSprite.RegisterRenderable( 2 );
	}

	{
		b2BodyDef bodyDefinition;
		bodyDefinition.userData = (IPhysics*)this;
		bodyDefinition.position = b2Vec2(GetPosition().x, GetPosition().y);
		bodyDefinition.angle = 0.0f;
		bodyDefinition.type = b2_kinematicBody;
		bodyDefinition.gravityScale = 9.0f;
		bodyDefinition.fixedRotation = true;

		_platformBody.CreateBody( bodyDefinition );

		b2PolygonShape boxShape;
		boxShape.SetAsBox( 0.5f*BLOCK_SIZE, 0.5f*BLOCK_SIZE );

		b2FixtureDef fixtureDefinition;
		fixtureDefinition.shape = &boxShape;
		fixtureDefinition.density = 1.0f;
		fixtureDefinition.friction = 0.5f;
		fixtureDefinition.restitution = 0.0f;

		_platformBody.CreateFixture( fixtureDefinition, "Block" );

		_platformBody.ResetTransform();
	}

	if(propertyElement)
	{
		{
			propertyElement->QueryFloatAttribute("TravelSpeed",&_travelSpeed);
		}

		{
			float totalDistance = 0.0f;
			const TiXmlElement* pathNode = propertyElement->FirstChildElement("node");
			_platformRoute.push_back(GetPosition());
			while(pathNode)
			{
				float x = 0.f, y = 0.f;
				pathNode->QueryFloatAttribute("x",&x);
				pathNode->QueryFloatAttribute("y",&y);

				Vec2D world((x - SCREENWIDTH/2) * UNRATIO, (y - SCREENHEIGHT/2) * UNRATIO * -1);

				totalDistance += Magnitude(world - _platformRoute.back());

				_platformRoute.push_back(world);

				pathNode = pathNode->NextSiblingElement();
			}

			_totalTravelTime = totalDistance / _travelSpeed;
		}

		{
			_platformRouteIterator = _platformRoute.begin();
		}
	}
}

const Vec2D& MovingPlatformEntity::GetNextPlatformDestination()
{
	_platformRouteIterator++;

	if(_platformRouteIterator == _platformRoute.end())
	{
		_platformRouteIterator = _platformRoute.begin();
	}

	return *_platformRouteIterator;
}
