#ifndef TASKMANAGER_HPP
#define TASKMANAGER_HPP

#include <list>
#include <unordered_map>
#include <queue>

#include "../Base/Singleton.hpp"
#include "../Interface/ITask.hpp"

class TaskManager : public Singleton<TaskManager>
{
    DEFINE_SINGLETON( TaskManager )

private:
    typedef std::list<ITask*> ToDoList;

public:
    void Update(float dt);
	void AddTask(ITask* task);
	void RemoveTask(ITask* task);
private:
    ToDoList _toDoList;
};

#endif