#ifndef LEVELCOMPLETEZONEENTITY_HPP
#define LEVELCOMPLETEZONEENTITY_HPP

#include "../Entity/Entity.hpp"
#include "../Entity/Component/BodyWrapper.hpp"

class LevelCompleteZoneEntity : public Entity
{
	DEFINE_ENTITY(LevelCompleteZoneEntity,Entity,'LVL')

public:
	virtual void Initialize( const TiXmlElement *propertyElement = NULL );
	virtual void ProcessContact(const b2Contact* contact, const b2Fixture* contactFixture );
	virtual bool HandleEvent( const EventData& theevent );

private:
	bool _activated;
	BodyWrapper _triggerBody;
};

#endif