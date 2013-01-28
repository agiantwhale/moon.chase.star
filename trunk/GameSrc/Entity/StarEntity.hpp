#ifndef STARENTITY_HPP
#define STARENTITY_HPP

#include "../Base/Spline.hpp"
#include "../Entity/Entity.hpp"
#include "../Entity/Component/SpriteWrapper.hpp"
#include "../Entity/Component/ParticleWrapper.hpp"

class StarEntity : public Entity
{
	DEFINE_ENTITY(StarEntity,Entity,'STAR')

public:
	virtual void Update(float deltaTime);
	virtual void Initialize( const TiXmlElement *propertyElement /* = nullptr */ );

	bool GetIsTraveling() const { return _isTraveling; }
	void SetIsTraveling(bool val) { _isTraveling = val; }

private:
	SpriteWrapper _starSprite;
	ParticleWrapper _starParticle;

	bool  _isTraveling;
	float _totalTravelTime;
	float _currentTime;
	Vec2D _previousPosition;

	Spline<float,Vec2D> _travelPath;
};

#endif