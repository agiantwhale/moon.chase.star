#include "Task.hpp"
#include "TaskManager.hpp"

namespace sb
{
	Task::Task( sf::Time taskDuration ) : m_timeRemaining(), m_taskState(kTask_Unregistered)
	{
		if(taskDuration > sf::Time::Zero)
		{
			m_timeRemaining.reset(taskDuration);
		}
		
	}

	void Task::start()
	{
		m_taskState = kTask_Started;
		m_timeRemaining.start();
	}

	bool Task::doTask( sf::Time deltaTime )
	{
		return m_timeRemaining.isExpired();
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
