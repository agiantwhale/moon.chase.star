#ifndef TaskManager_h__
#define TaskManager_h__

#include <list>
#include <unordered_map>
#include <queue>

#include "../Base/Singleton.hpp"
#include "../Event/EventListener.hpp"

namespace sb
{
	class Task;

	class TaskManager : public Singleton<TaskManager>, public EventListener
	{
		DEFINE_SINGLETON( TaskManager )

	private:
		typedef std::list<Task*> ToDoList;

	public:
		void update(float dt);
		void addTask(Task* task);
		void removeTask(Task* task);
	private:
		virtual bool handleEvent(const EventData& theevent);

		ToDoList _toDoList;
	};
}


#endif // TaskManager_h__
