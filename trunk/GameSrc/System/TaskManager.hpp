#ifndef TASKMANAGER_HPP
#define TASKMANAGER_HPP

#include <list>
#include <unordered_map>
#include <queue>

#include "../Base/Singleton.hpp"
#include "../Interface/IEventListener.hpp"

class Task;

class TaskManager : public Singleton<TaskManager>, public IEventListener
{
    DEFINE_SINGLETON( TaskManager )

private:
    typedef std::list<Task*> ToDoList;

public:
    void Update(float dt);
	void AddTask(Task* task);
	void RemoveTask(Task* task);
private:
	virtual bool HandleEvent(const EventData& theevent);

    ToDoList _toDoList;
};

#endif