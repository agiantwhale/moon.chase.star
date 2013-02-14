#ifndef ZONEENTITY_HPP
#define ZONEENTITY_HPP

#include <list>
#include "../Entity/Entity.hpp"
#include "../Entity/Component/BodyWrapper.hpp"

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
	bool handleEvent(const EventData& theevent);
	void postLoad();

private:
	BodyWrapper _zoneBody;
	bool		_containsBall;
	static std::list<Task*> _taskList;

	friend class sb::SceneManager;
	friend class sb::TaskManager;
	friend class CameraMoveTask;
};

#endif