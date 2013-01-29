#include "../Entity/TeleportEntity.hpp"
#include "../Event/ContactEventData.h"

REGISTER_ENTITY(TeleportEntity,"Teleporter")

const float ROTATION_PER_SECOND = 10.0f;

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
}
