#ifndef ZoneEntity_h__
#define ZoneEntity_h__

#include <list>
#include "Entity.hpp"
#include "../Task/Task.hpp"
#include "../Physics/BodyController.hpp"

namespace sb
{
	class SceneManager;
	class TaskManager;
	class Task;
}

class CameraMoveTask;

class ZoneEntity : public sb::Entity
{
	DEFINE_ENTITY( ZoneEntity, sb::Entity, 'ZONE')

public:
	void initializeEntity( const TiXmlElement *propertyElement = nullptr );
	bool handleEvent(const sb::EventData& theevent);
	void postLoad();

private:
	sb::BodyController	m_zoneBody;
	bool				m_containsBall;
	static std::list<sb::Task*> m_taskList;

	friend class sb::SceneManager;
	friend class sb::TaskManager;
	friend class CameraMoveTask;
};

#endif // ZoneEntity_h__
