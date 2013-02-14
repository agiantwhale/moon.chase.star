#ifndef ZONEENTITY_HPP
#define ZONEENTITY_HPP

#include <list>
#include "../Entity/Entity.hpp"
#include "../Entity/Component/BodyWrapper.hpp"

class SceneManager;
class Task;
class CameraMoveTask;
class TaskManager;

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
	friend class CameraMoveTask;
};

#endif