#include "../Task/TeleportTask.hpp"
#include <algorithm>

TeleportTask::TeleportTask( float taskDuration, ITransform* targetTransform, b2Body* targetBody, const Vec2D& destination )
	:	Task(taskDuration),
		_totalSeconds(taskDuration),
		_targetTransform(targetTransform),
		_targetBody(targetBody),
		_destination(destination),
		_movementSpeed(0.f,0.f)
{
}

void TeleportTask::Start()
{
	_movementSpeed = (_destination = _targetTransform->GetPosition())/_totalSeconds;
}

bool TeleportTask::DoTask( float deltaTime )
{
	if(Task::DoTask(deltaTime))
		return true;

	float scale = std::max<float>(GetTimeRemaining()/_totalSeconds,0.0f);

	_targetTransform->SetScale(Vec2D(scale));
	_targetTransform->SetPosition(_targetTransform->GetPosition() + _movementSpeed * deltaTime);

	return false;
}

void TeleportTask::End()
{
	_targetTransform->SetScale(Vec2D(1.0f));
	_targetTransform->SetPosition(_destination);
}
