#include "TaskManager.hpp"
#include "../Task/Task.hpp"
#include "../Entity/ZoneEntity.hpp"

namespace sb
{
	SINGLETON_CONSTRUCTOR(TaskManager), EventListener()
	{
		AddEventListenType(Event_Unload);
		AddEventListenType(Event_RestartLevel);

		_toDoList.clear();
	}

	SINGLETON_DESTRUCTOR(TaskManager)
	{
		RemoveEventListenType(Event_Unload);
		RemoveEventListenType(Event_RestartLevel);
	}

	void TaskManager::removeTask( Task* task )
	{
		_toDoList.remove(task);
		task->_taskState = Task::kTask_Unregistered;
	}

	void TaskManager::update( float dt )
	{
		for(ToDoList::iterator iter = _toDoList.begin(); iter != _toDoList.end();)
		{
			Task* task = (*iter);

			if(task->_taskState == Task::kTask_Registered)
			{
				task->Start();
			}

			if(task->DoTask(dt))
			{
				task->End();
				delete task;
				iter = _toDoList.erase(iter);
			}
			else
			{
				iter++;
			}
		}
	}

	void TaskManager::addTask( Task* task )
	{
		_toDoList.push_back(task);
		task->_taskState = Task::kTask_Registered;
	}

	bool TaskManager::handleEvent( const EventData& theevent )
	{
		if(theevent.getEventType() == Event_Unload || theevent.getEventType() == Event_RestartLevel)
		{
			for(ToDoList::iterator iter = _toDoList.begin(); iter != _toDoList.end(); iter++)
			{
				Task* task = (*iter);
				delete task;
			}

			ZoneEntity::_taskList.clear();

			_toDoList.clear();
		}

		return false;
	}
}