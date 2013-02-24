#include <Thor/Vectors.hpp>
#include "LevelCompleteTask.hpp"
#include "../Event/EventData.hpp"
#include "../Entity/PlayerEntity.hpp"
#include "../Base/Math.hpp"

const float MOVE_SPEED = 3.0f;

sf::Time CalculateTravelTime(const sf::Vector2f& initialPosition, const VectorStack& path)
{
	float totalTravelLength = 0.0f;
	sf::Vector2f previousNode = initialPosition;
	for(VectorStack::const_iterator iter = path.begin(); iter != path.end(); iter++)
	{
		sf::Vector2f currentNode = (*iter);
		totalTravelLength += thor::length<float>(currentNode - previousNode);
		previousNode = currentNode;
	}

	return sf::seconds(totalTravelLength/MOVE_SPEED);
}

LevelCompleteTask::LevelCompleteTask(PlayerEntity* playerEntity, const VectorStack& path)
	:	sb::Task(CalculateTravelTime(playerEntity->getPosition(), path)),
		m_playerEntity(playerEntity),
		m_path(path),
		m_totalTime(0.f),
		m_xSpline(),
		m_ySpline(),
		m_currentState(kLevelComplete_Spline)
{
}

void LevelCompleteTask::start()
{
	sb::Task::start();

	m_playerEntity->getBallBody()->SetActive(false);

	//_totalLerpTime = Magnitude(_playerEntity->GetPosition() - (*_path.begin())) / MOVE_SPEED;
	//_lerpTimer = _totalLerpTime;

	m_xSpline.addPoint(m_totalTime,m_playerEntity->getPosition().x);
	m_ySpline.addPoint(m_totalTime,m_playerEntity->getPosition().y);

	sf::Vector2f previousNode = m_playerEntity->getPosition();
	for(VectorStack::const_iterator iter = m_path.begin(); iter != m_path.end(); iter++)
	{
		sf::Vector2f currentNode = (*iter);

		{
			m_totalTime += thor::length<float>(currentNode - previousNode)/MOVE_SPEED;
			m_xSpline.addPoint(m_totalTime,currentNode.x);
			m_ySpline.addPoint(m_totalTime,currentNode.y);
		}

		previousNode = currentNode;
	}
}

bool LevelCompleteTask::doTask( sf::Time deltaTime )
{
	if(sb::Task::doTask(deltaTime))
		return true;

	switch(m_currentState)
	{
	case kLevelComplete_Spline:
		{
			if(getRemainingTime() <= sf::Time::Zero)
			{
				m_currentState = kLevelComplete_Arrived;
			}

			m_playerEntity->setPosition( m_xSpline(m_totalTime-getRemainingTime().asSeconds()), m_ySpline(m_totalTime-getRemainingTime().asSeconds()) );

			break;
		}

	case kLevelComplete_Arrived:
		{
			return true;
			break;
		}
	}

	return false;
}

void LevelCompleteTask::end()
{
	sb::Task::end();

	sb::EventData* eventData = new sb::EventData(Event_ShowGameWon);
	eventData->queueEvent(sf::seconds(1.5f));
}
