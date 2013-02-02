#include "../System/TaskManager.hpp"
#include "../Task/Task.hpp"

SINGLETON_CONSTRUCTOR(TaskManager), IEventListener("TaskManager")
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

void TaskManager::RemoveTask( Task* task )
{
	_toDoList.remove(task);
	task->_taskState = Task::kTask_Unregistered;
}

void TaskManager::Update( float dt )
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

void TaskManager::AddTask( Task* task )
{
	_toDoList.push_back(task);
	task->_taskState = Task::kTask_Registered;
}

bool TaskManager::HandleEvent( const EventData& theevent )
{
	if(theevent.GetEventType() == Event_Unload || theevent.GetEventType() == Event_RestartLevel)
	{
		for(ToDoList::iterator iter = _toDoList.begin(); iter != _toDoList.end(); iter++)
		{
			Task* task = (*iter);
			delete task;
		}

		_toDoList.clear();
	}

	return false;
}

