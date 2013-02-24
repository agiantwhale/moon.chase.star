#include "Task.hpp"
#include "TaskManager.hpp"

namespace sb
{
	Task::Task( sf::Time taskDuration ) : m_timeRemaining(taskDuration), m_taskState(kTask_Unregistered)
	{	
	}

	void Task::start()
	{
		m_taskState = kTask_Started;
	}

	bool Task::doTask( sf::Time deltaTime )
	{
		m_timeRemaining -= deltaTime;

		return (m_timeRemaining <= sf::Time::Zero);
	}

	void Task::end()
	{
		m_taskState = kTask_Ended;
	}

	void Task::addTask()
	{
		TaskManager::getInstance()->addTask(this);
	}

	void Task::removeTask()
	{
		TaskManager::getInstance()->removeTask(this);
	}

}
