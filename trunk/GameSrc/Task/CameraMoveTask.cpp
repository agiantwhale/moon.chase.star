#include "../Base/Math.hpp"
#include "../Entity/ZoneEntity.hpp"
#include "../Task/CameraMoveTask.hpp"
#include "../System/GraphicsManager.hpp"

const float HORIZONTAL_TRAVEL_SPEED = 20.0f;
const float VERTICAL_TRAVEL_SPEED = 22.5f;

float CalculateTravelTime(const Vec2D& deltaDistance, float affector)
{
	if(deltaDistance.x != 0)
	{
		return std::abs(deltaDistance.x) / (HORIZONTAL_TRAVEL_SPEED*affector);
	}

	if(deltaDistance.y != 0)
	{
		return std::abs(deltaDistance.y) / (VERTICAL_TRAVEL_SPEED*affector);
	}

	return 0;
}

CameraMoveTask::CameraMoveTask( const Vec2D& moveDistance, unsigned int renderLayer, float affector ) : Task( CalculateTravelTime(moveDistance, affector) ),
																													_renderLayer(renderLayer),
																													_initialPosition(GraphicsManager::GetInstance()->GetRenderLayer(_renderLayer)->GetCamera().GetPosition()),
																													_finalDestination(_initialPosition + moveDistance),
																													_affector(affector),
																													_moveSpeed(HORIZONTAL_TRAVEL_SPEED,VERTICAL_TRAVEL_SPEED)
{
}

void CameraMoveTask::Start()
{
	Task::Start();

	Vec2D deltaDistance = (_finalDestination - _initialPosition);
	_moveSpeed.x *= signum<float>(deltaDistance.x);
	_moveSpeed.y *= signum<float>(deltaDistance.y);
}

bool CameraMoveTask::DoTask( float deltaTime )
{
	if(Task::DoTask(deltaTime))
		return true;

	GraphicsManager::GetInstance()->GetRenderLayer(_renderLayer)->GetCamera().SetPosition(GraphicsManager::GetInstance()->GetRenderLayer(_renderLayer)->GetCamera().GetPosition()+_moveSpeed*_affector*deltaTime);

	return false;
}

void CameraMoveTask::End()
{
	Task::End();

	ZoneEntity::_taskList.remove(this);
	GraphicsManager::GetInstance()->GetRenderLayer(_renderLayer)->GetCamera().SetPosition(_finalDestination);
}