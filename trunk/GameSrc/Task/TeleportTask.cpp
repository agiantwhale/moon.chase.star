#include "../Task/TeleportTask.hpp"
#include "../System/ResourceCache.hpp"
#include <algorithm>

TeleportTask::TeleportTask( float taskDuration, PlayerEntity* playerEntity, TeleportEntity* teleportEntity )
	:	Task(taskDuration),
		_totalSeconds(taskDuration),
		_playerEntity(playerEntity),
		_teleportEntity(teleportEntity),
		_movementSpeed(0.f,0.f),
		_teleportSound(nullptr)
{
	{
		thor::ResourceKey<sf::SoundBuffer> key = thor::Resources::fromFile<sf::SoundBuffer>("Resource/Sounds/teleport.wav");
		std::shared_ptr<sf::SoundBuffer> buffer = ResourceCache::GetInstance()->acquire<sf::SoundBuffer>(key);
		_teleportSound = new sf::Sound(*buffer);
	}
}

void TeleportTask::Start()
{
	Task::start();

	_movementSpeed = (_teleportEntity->GetPosition() - _playerEntity->GetPosition())/_totalSeconds;
	_playerEntity->GetBallBody().GetBody()->SetActive(false);

	_teleportSound->play();
}

bool TeleportTask::DoTask( sf::Time deltaTime )
{
	if(Task::doTask(deltaTime))
		return true;

	float scale = std::max<float>(GetTimeRemaining()/_totalSeconds,0.0f);

	_playerEntity->SetScale(Vec2D(scale));
	_playerEntity->SetPosition(_playerEntity->GetPosition() + _movementSpeed * deltaTime);

	return false;
}

void TeleportTask::End()
{
	Task::end();

	Vec2D exitPos = _teleportEntity->GetExitPosition();
	_playerEntity->SetScale(Vec2D(1.0f));
	_playerEntity->GetBallBody().GetBody()->SetActive(true);
	_playerEntity->GetBallBody().GetBody()->SetAngularVelocity(0.0f);
	_playerEntity->GetBallBody().GetBody()->SetLinearVelocity(b2Vec2(0.0f,0.0f));
	_playerEntity->GetBallBody().GetBody()->SetTransform(exitPos,_playerEntity->GetBallBody().GetBody()->GetAngle());
	_playerEntity->GetBallBody().GetBody()->SetAwake(true);
	_playerEntity->GetBallBody().ResetTransform();
	_playerEntity->GetBallBody().UpdateTransform();
	_playerEntity->Fall();
}

TeleportTask::~TeleportTask()
{
	delete _teleportSound;
}
