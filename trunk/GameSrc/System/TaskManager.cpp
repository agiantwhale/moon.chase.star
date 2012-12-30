#include "../System/TaskManager.hpp"

SINGLETON_CONSTRUCTOR(TaskManager)
{
	_toDoList.clear();
}

SINGLETON_DESTRUCTOR(TaskManager)
{

}

void TaskManager::RemoveTask( ITask* task )
{
	_toDoList.remove(task);
}

void TaskManager::Update( float dt )
{
	for(ToDoList::iterator iter = _toDoList.begin(); iter != _toDoList.end();)
	{
		ITask* task = (*iter);

		if(task->DoTask(dt))
		{
			task->End();
			iter = _toDoList.erase(iter);
		}
		else
		{
			iter++;
		}
	}
}

void TaskManager::AddTask( ITask* task )
{
	_toDoList.push_back(task);
}

