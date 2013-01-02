#include "../Task/Task.hpp"
#include "../System/TaskManager.hpp"

Task::Task( float taskDuration ) : _timeRemaining(taskDuration)
{

}

void Task::Start()
{

}

bool Task::DoTask( float deltaTIme )
{
	_timeRemaining -= deltaTIme;

	return _timeRemaining <= 0;
}

void Task::End()
{

}

void Task::AddTask()
{
	TaskManager::GetInstance()->AddTask(this);
}

void Task::RemoveTask()
{
	TaskManager::GetInstance()->RemoveTask(this);
}

