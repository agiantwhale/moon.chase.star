#ifndef Task_h__
#define Task_h__

#include <Thor/Time/Timer.hpp>
#include <SFML/System/Time.hpp>

namespace sb
{
	class TaskManager;

	class Task
	{
	public:
		enum TaskState
		{
			kTask_Unregistered,
			kTask_Registered,
			kTask_Started,
			kTask_Ended
		};

		Task(sf::Time taskDuration);
		virtual ~Task() {}

		void addTask();
		void removeTask();

		virtual void start();
		//Return true when task is over.
		virtual bool doTask(sf::Time deltaTime);
		virtual void end();

	protected:
		const thor::Timer& getTaskTimer() const { return m_timeRemaining; }
		float getRemainingTime() const { return m_timeRemaining.getRemainingTime().asSeconds(); }

	private:
		thor::Timer m_timeRemaining;
		TaskState	m_taskState;

		friend class TaskManager;
	};
}

#endif // Task_h__
