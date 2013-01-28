#include <boost/lexical_cast.hpp>
#include "../Entity/StarEntity.hpp"

REGISTER_ENTITY(StarEntity,"Star")

StarEntity::StarEntity()
	:	BaseClass(),
		_starSprite(this),
		_starParticle(this),
		_isTraveling(false),
		_totalTravelTime(0.f),
		_currentTime(0.f),
		_previousPosition(0.f,0.f),
		_travelPath()
{

}

StarEntity::~StarEntity()
{

}

void StarEntity::Update( float deltaTime )
{
	BaseClass::Update(deltaTime);

	_currentTime += deltaTime;
}

void StarEntity::Initialize( const TiXmlElement *propertyElement /* = nullptr */ )
{
	BaseClass::Initialize(propertyElement);

	if( propertyElement )
	{
		{
			propertyElement->QueryFloatAttribute("TotalTravelTime",&_totalTravelTime);
		}

		{
			
		}
	}
}

