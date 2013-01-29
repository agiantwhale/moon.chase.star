#ifndef STARENTITY_HPP
#define STARENTITY_HPP

#include "../Base/Math.hpp"
#include "../Entity/Entity.hpp"
#include "../Entity/Component/SpriteWrapper.hpp"
#include "../Entity/Component/ParticleWrapper.hpp"

class StarEntity : public Entity
{
	DEFINE_ENTITY(StarEntity,Entity,'STAR')

public:
	virtual void Update(float deltaTime);
	virtual void Initialize( const TiXmlElement *propertyElement /* = nullptr */ );

private:
	SpriteWrapper _starSprite;
	ParticleWrapper _starParticle;

	enum 
	{
		kStar_Traveling,
		kStar_Arrived
	} _starState;
	float _currentTime;
	float _totalTravelTime;
	Vec2D _previousPosition;
	Vec2D _particleVelocity;

	Spline _xSpline;
	Spline _ySpline;

	thor::UniversalEmitter::Ptr _emitter;
};

#endif