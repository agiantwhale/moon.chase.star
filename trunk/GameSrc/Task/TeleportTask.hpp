#ifndef TeleportTask_h__
#define TeleportTask_h__

#include <SFML/System/Vector2.hpp>
#include <SFML/Audio.hpp>
#include <SFML/System/Time.hpp>
#include <Thor/Time/Timer.hpp>
#include "Task.hpp"

class PlayerEntity;
class TeleportEntity;

class TeleportTask : public sb::Task
{
public:
	TeleportTask( sf::Time taskDuration, PlayerEntity* playerEntity, TeleportEntity* teleportEntity );

	virtual void start();
	virtual bool doTask(sf::Time deltaTIme);
	virtual void end();

private:
	PlayerEntity*		m_playerEntity;
	TeleportEntity*		m_teleportEntity;
	sf::Vector2f		m_movementSpeed;
	sf::Sound			m_teleportSound;
	sf::Time			m_totalTime;
};

#endif // TeleportTask_h__
