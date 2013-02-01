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
	Task::Start();

	_initialPosition = GraphicsManager::GetInstance()->GetRenderLayer(_renderLayer)->GetCamera().GetPosition();
}

bool CameraMoveTask::DoTask( float deltaTime )
{
	if(Task::DoTask(deltaTime))
		return true;

	GraphicsManager::GetInstance()->GetRenderLayer(_renderLayer)->GetCamera().SetPosition(GraphicsManager::GetInstance()->GetRenderLayer(_renderLayer)->GetCamera().GetPosition()+_moveSpeed*deltaTime);

	return false;
}

void CameraMoveTask::End()
{
	Task::End();

	GraphicsManager::GetInstance()->GetRenderLayer(_renderLayer)->GetCamera().SetPosition(_initialPosition + _totalMoveAmount );
}

