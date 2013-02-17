#include "TaskManager.hpp"
#include "../Task/Task.hpp"
#include "../Entity/ZoneEntity.hpp"

namespace sb
{
	SINGLETON_CONSTRUCTOR(TaskManager), EventListener()
	{
		addEventListenType(Event_Unload);
		addEventListenType(Event_RestartLevel);

		m_toDoList.clear();
	}

	SINGLETON_DESTRUCTOR(TaskManager)
	{
		removeEventListenType(Event_Unload);
		removeEventListenType(Event_RestartLevel);
	}

	void TaskManager::removeTask( Task* task )
	{
		m_toDoList.remove(task);
		task->m_taskState = Task::kTask_Unregistered;
	}

	void TaskManager::update( sf::Time deltaTime )
	{
		for(ToDoList::iterator iter = m_toDoList.begin(); iter != m_toDoList.end();)
		{
			Task* task = (*iter);

			if(task->m_taskState == Task::kTask_Registered)
			{
				task->start();
			}

			if(task->doTask(deltaTime))
			{
				task->end();
				delete task;
				iter = m_toDoList.erase(iter);
			}
			else
			{
				iter++;
			}
		}
	}

	void TaskManager::addTask( Task* task )
	{
		m_toDoList.push_back(task);
		task->m_taskState = Task::kTask_Registered;
	}

	bool TaskManager::handleEvent( const EventData& theevent )
	{
		if(theevent.getEventType() == Event_Unload || theevent.getEventType() == Event_RestartLevel)
		{
			for(ToDoList::iterator iter = m_toDoList.begin(); iter != m_toDoList.end(); iter++)
			{
				Task* task = (*iter);
				delete task;
			}

			ZoneEntity::m_taskList.clear();

			m_toDoList.clear();
		}

		return false;
	}
}