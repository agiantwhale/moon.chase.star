#ifndef ZONEENTITY_HPP
#define ZONEENTITY_HPP

#include <list>
#include "../Entity/Entity.hpp"
#include "../Entity/Component/BodyWrapper.hpp"

class SceneManager;
class Task;
class CameraMoveTask;
class TaskManager;

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
	static std::list<Task*> _taskList;

	friend class SceneManager;
	friend class CameraMoveTask;
	friend class TaskManager;
};

#endif