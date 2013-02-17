#include "../Base/Math.hpp"
#include "CameraMoveTask.hpp"
#include "../Entity/ZoneEntity.hpp"
#include "../system/RenderLayer.hpp"
#include "../System/GraphicsManager.hpp"

const float HORIZONTAL_TRAVEL_SPEED = 20.0f;
const float VERTICAL_TRAVEL_SPEED = 22.5f;

sf::Time CalculateTravelTime(const sf::Vector2f& deltaDistance, float affector)
{
	float totalTravelTime = 0.f;

	if(deltaDistance.x != 0)
	{
		totalTravelTime = std::abs(deltaDistance.x) / (HORIZONTAL_TRAVEL_SPEED*affector);
	}

	if(deltaDistance.y != 0)
	{
		totalTravelTime = std::abs(deltaDistance.y) / (VERTICAL_TRAVEL_SPEED*affector);
	}

	return sf::seconds(totalTravelTime);
}

CameraMoveTask::CameraMoveTask( const sf::Vector2f& moveDistance, unsigned int renderLayer, float affector ) : Task( CalculateTravelTime(moveDistance, affector) ),
																													m_renderLayer(renderLayer),
																													m_initialPosition(sb::GraphicsManager::getInstance()->getRenderLayer(m_renderLayer)->getCamera().getPosition()),
																													m_finalDestination(m_initialPosition + moveDistance),
																													m_affector(affector),
																													m_moveSpeed(HORIZONTAL_TRAVEL_SPEED,VERTICAL_TRAVEL_SPEED)
{
}

void CameraMoveTask::start()
{
	sb::Task::start();

	sf::Vector2f deltaDistance = (m_finalDestination - m_initialPosition);
	m_moveSpeed.x *= sb::signum<float>(deltaDistance.x);
	m_moveSpeed.y *= sb::signum<float>(deltaDistance.y);
}

bool CameraMoveTask::doTask( sf::Time deltaTime )
{
	if(sb::Task::doTask(deltaTime))
		return true;

	sb::GraphicsManager::getInstance()->getRenderLayer(m_renderLayer)->getCamera().setPosition(sb::GraphicsManager::getInstance()->getRenderLayer(m_renderLayer)->getCamera().getPosition()+m_moveSpeed*m_affector*deltaTime.asSeconds());

	return false;
}

void CameraMoveTask::end()
{
	Task::end();

	ZoneEntity::m_taskList.remove(this);
	sb::GraphicsManager::getInstance()->getRenderLayer(m_renderLayer)->getCamera().setPosition(m_finalDestination);
}