#ifndef ZONEENTITY_HPP
#define ZONEENTITY_HPP

#include "../Entity/Entity.hpp"
#include "../Entity/Component/BodyWrapper.hpp"

class SceneManager;

class ZoneEntity : public Entity
{
	DEFINE_ENTITY( ZoneEntity, Entity, 'ZONE')

public:
	void Initialize( const TiXmlElement *propertyElement = nullptr );
	bool HandleEvent(const EventData& theevent);
	void PostLoad();

private:
	BodyWrapper _zoneBody;
	bool		_containsBall;

	friend class SceneManager;
};

#endif