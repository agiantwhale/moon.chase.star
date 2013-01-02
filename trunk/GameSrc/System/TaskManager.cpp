#include "../System/TaskManager.hpp"
#include "../Task/Task.hpp"

SINGLETON_CONSTRUCTOR(TaskManager), IEventListener("TaskManager")
{
	AddEventListenType(Event_Unload);

	_toDoList.clear();
}

SINGLETON_DESTRUCTOR(TaskManager)
{

}

void TaskManager::RemoveTask( Task* task )
{
	_toDoList.remove(task);
}

void TaskManager::Update( float dt )
{
	for(ToDoList::iterator iter = _toDoList.begin(); iter != _toDoList.end();)
	{
		Task* task = (*iter);

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
}

bool TaskManager::HandleEvent( const EventData& theevent )
{
	if(theevent.GetEventType() == Event_Unload)
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

