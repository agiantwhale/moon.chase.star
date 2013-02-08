#ifndef ZONEENTITY_HPP
#define ZONEENTITY_HPP

#include <list>
#include "../Entity/Entity.hpp"
#include "../Entity/Component/BodyWrapper.hpp"

class DeadZoneEntity : public Entity
{
	DEFINE_ENTITY( DeadZoneEntity, Entity, 'DEAD')

public:
	void Initialize( const TiXmlElement *propertyElement = nullptr );
	bool HandleEvent(const EventData& theevent);

private:
	BodyWrapper _zoneBody;
};

#endif