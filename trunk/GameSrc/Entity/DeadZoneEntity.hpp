#ifndef DeadZoneEntity_h__
#define DeadZoneEntity_h__

#include "Entity.hpp"
#include "../Physics/BodyController.hpp"

class DeadZoneEntity : public sb::Entity
{
	DEFINE_ENTITY( DeadZoneEntity, sb::Entity, 'DEAD')

private:
	void initializeEntity( const TiXmlElement *propertyElement = nullptr );
	bool handleEvent(const sb::EventData& theevent);

	sb::BodyController	m_zoneBody;
};

#endif // DeadZoneEntity_h__
