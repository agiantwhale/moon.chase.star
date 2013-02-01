#include "../Task/TeleportTask.hpp"
#include <algorithm>

TeleportTask::TeleportTask( float taskDuration, PlayerEntity* playerEntity, TeleportEntity* teleportEntity )
	:	Task(taskDuration),
		_totalSeconds(taskDuration),
		_playerEntity(playerEntity),
		_teleportEntity(teleportEntity),
		_movementSpeed(0.f,0.f)
{
}

void TeleportTask::Start()
{
	Task::Start();

	_movementSpeed = (_teleportEntity->GetPosition() - _playerEntity->GetPosition())/_totalSeconds;
	_playerEntity->GetBallBody().GetBody()->SetActive(false);
}

bool TeleportTask::DoTask( float deltaTime )
{
	if(Task::DoTask(deltaTime))
		return true;

	float scale = std::max<float>(GetTimeRemaining()/_totalSeconds,0.0f);

	_playerEntity->SetScale(Vec2D(scale));
	_playerEntity->SetPosition(_playerEntity->GetPosition() + _movementSpeed * deltaTime);

	return false;
}

void TeleportTask::End()
{
	Task::End();

	Vec2D exitPos = _teleportEntity->GetExitPosition();
	_playerEntity->SetScale(Vec2D(1.0f));
	_playerEntity->GetBallBody().GetBody()->SetActive(true);
	_playerEntity->GetBallBody().GetBody()->SetTransform(exitPos,_playerEntity->GetBallBody().GetBody()->GetAngle());
	_playerEntity->GetBallBody().GetBody()->SetAwake(true);
	_playerEntity->GetBallBody().ResetTransform();
	_playerEntity->GetBallBody().UpdateTransform();
	_playerEntity->Fall();
}
