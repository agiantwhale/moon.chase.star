#ifndef TELEPORTENTITY_HPP
#define TELEPORTENTITY_HPP

#include "../Entity/Entity.hpp"
#include "../Entity/Component/SpriteWrapper.hpp"
#include "../Entity/Component/BodyWrapper.hpp"

class TeleportEntity : public Entity
{
	DEFINE_ENTITY( TeleportEntity, Entity, 'TLPT')

public:
	virtual void Update(float deltaTime);
	virtual void Initialize( const TiXmlElement *propertyElement = NULL );

	const Vec2D& GetExitPosition() {return _exitTransform.GetPosition();}
private:
	ITransform		_enterTransform;
	ITransform		_exitTransform;

	BodyWrapper		_triggerBody;
	SpriteWrapper	_enterSprite;
	SpriteWrapper	_exitSprite;
};

#endif