#include "../Interface/ITask.hpp"

ITask::ITask( float taskDuration ) : _timeRemaining(taskDuration)
{

}

bool ITask::DoTask( float deltaTIme )
{
	_timeRemaining -= deltaTIme;
	return _timeRemaining <= 0.0f;
}
