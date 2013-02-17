#ifndef HullEntity_h__
#define HullEntity_h__

#include "../Entity/Entity.hpp"
#include "../Physics/BodyController.hpp"

class HullEntity : public sb::Entity
{
	DEFINE_ENTITY( HullEntity, sb::Entity, 'HULL')

private:
	virtual void initializeEntity( const TiXmlElement *propertyElement = NULL );

	sb::BodyController m_hullBody;
};

#endif // HullEntity_h__
