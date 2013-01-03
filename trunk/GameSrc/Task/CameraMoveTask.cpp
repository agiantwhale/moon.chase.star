#include "../Task/CameraMoveTask.hpp"
#include "../System/GraphicsManager.hpp"

CameraMoveTask::CameraMoveTask( float timeDuration, unsigned int renderLayer, const Vec2D& moveSpeed ) : Task(timeDuration),
																										 _renderLayer(renderLayer),
																										 _initialPosition(),
																										 _totalMoveAmount(moveSpeed * timeDuration),
																										 _moveSpeed(moveSpeed)
{
}

void CameraMoveTask::Start()
{
	_initialPosition = GraphicsManager::GetInstance()->GetRenderLayer(_renderLayer)->GetCamera().GetPosition();
}

bool CameraMoveTask::DoTask( float deltaTIme )
{
	if(Task::DoTask(deltaTIme))
		return true;

	GraphicsManager::GetInstance()->GetRenderLayer(_renderLayer)->GetCamera().SetPosition(GraphicsManager::GetInstance()->GetRenderLayer(_renderLayer)->GetCamera().GetPosition()+_moveSpeed*deltaTIme);

	return false;
}

void CameraMoveTask::End()
{
	GraphicsManager::GetInstance()->GetRenderLayer(_renderLayer)->GetCamera().SetPosition(_initialPosition + _totalMoveAmount );
}

