#include "../Task/Task.hpp"
#include "../System/TaskManager.hpp"

Task::Task( float taskDuration ) : _timeRemaining(taskDuration), _taskState(kTask_Unregistered)
{

}

void Task::Start()
{
	_taskState = kTask_Started;
}

bool Task::DoTask( float deltaTime )
{
	_timeRemaining -= deltaTime;

	return _timeRemaining <= 0;
}

void Task::End()
{
	_taskState = kTask_Ended;
}

void Task::AddTask()
{
	TaskManager::GetInstance()->AddTask(this);
}

void Task::RemoveTask()
{
	TaskManager::GetInstance()->RemoveTask(this);
}
