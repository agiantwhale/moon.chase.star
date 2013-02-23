#include "TeleportTask.hpp"
#include "../Helper/Conversion.hpp"
#include "../System/ResourceCache.hpp"
#include "../Entity/PlayerEntity.hpp"
#include "../Entity/TeleportEntity.hpp"
#include <algorithm>

TeleportTask::TeleportTask( sf::Time taskDuration, PlayerEntity* playerEntity, TeleportEntity* teleportEntity )
	:	sb::Task(taskDuration),
		m_playerEntity(playerEntity),
		m_teleportEntity(teleportEntity),
		m_movementSpeed(0.f,0.f),
		m_teleportSound(),
		m_totalTime(taskDuration)
{
	{
		thor::ResourceKey<sf::SoundBuffer> key = thor::Resources::fromFile<sf::SoundBuffer>("Resource/Sounds/teleport.wav");
		std::shared_ptr<sf::SoundBuffer> buffer = sb::ResourceCache::getInstance()->acquire<sf::SoundBuffer>(key);
		m_teleportSound.setBuffer(*buffer);
	}
}

void TeleportTask::start()
{
	sb::Task::start();

	m_movementSpeed = (m_teleportEntity->getPosition() - m_playerEntity->getPosition())/m_totalTime.asSeconds();
	m_playerEntity->getBallBody()->SetActive(false);
	m_teleportSound.play();
}

bool TeleportTask::doTask( sf::Time deltaTime )
{
	if(sb::Task::doTask(deltaTime))
		return true;

	float scale = std::max<float>(getRemainingTime()/m_totalTime.asSeconds(),0.0f);

	m_playerEntity->setScale(scale,scale);
	m_playerEntity->setPosition(m_playerEntity->getPosition() + m_movementSpeed * deltaTime.asSeconds());

	return false;
}

void TeleportTask::end()
{
	sb::Task::end();

	sf::Vector2f exitPos = m_teleportEntity->getExitPosition();
	m_playerEntity->setTeleportTask(nullptr);
	m_playerEntity->setScale(1.f,1.f);
	m_playerEntity->getBallBody()->SetActive(true);
	m_playerEntity->getBallBody()->SetTransform(ToVector(exitPos),m_playerEntity->getBallBody()->GetAngle());
	m_playerEntity->getBallBody()->SetAwake(true);
	m_playerEntity->resetBodyPosition();
	m_playerEntity->fall();
}