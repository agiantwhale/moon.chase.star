#ifndef MOVINGPLATFORMENTITY_HPP
#define MOVINGPLATFORMENTITY_HPP

#include "../Entity/Entity.hpp"
#include "../Entity/Component/SpriteWrapper.hpp"
#include "../Entity/Component/BodyWrapper.hpp"
#include <vector>

class MovingPlatformEntity : public Entity
{
	DEFINE_ENTITY(MovingPlatformEntity,Entity,'MVPT')

public:
	virtual void update(sf::Time deltaTime);
	virtual void initializeEntity( const TiXmlElement *propertyElement /* = nullptr */ );

	BodyWrapper GetPlatformBody() const { return _platformBody; }

private:

	const Vec2D& GetNextPlatformDestination();

	SpriteWrapper	_platformSprite;
	BodyWrapper		_platformBody;

	std::vector<Vec2D> _platformRoute;
	std::vector<Vec2D>::iterator _platformRouteIterator;
	float			_travelSpeed;
	float			_totalTravelTime;
	float			_currentTravelTime;
};

#endif