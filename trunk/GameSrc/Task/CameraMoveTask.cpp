#include "../Task/CameraMoveTask.hpp"
#include "../System/GraphicsManager.hpp"

CameraMoveTask::CameraMoveTask( float timeDuration, unsigned int renderLayer, const Vec2D& moveSpeed ) : Task(timeDuration),
																										 _targetCamera(nullptr),
																										 _renderLayer(renderLayer),
																										 _initialPosition(),
																										 _totalMoveAmount(moveSpeed * timeDuration),
																										 _moveSpeed(moveSpeed)
{
}

void CameraMoveTask::Start()
{
	_targetCamera = &GraphicsManager::GetInstance()->GetRenderLayer(_renderLayer)->GetCamera();
	_initialPosition = _targetCamera->GetPosition();
}

bool CameraMoveTask::DoTask( float deltaTIme )
{
	if(Task::DoTask(deltaTIme))
		return true;

	_targetCamera->SetPosition(_targetCamera->GetPosition()+_moveSpeed*deltaTIme);

	return false;
}

void CameraMoveTask::End()
{
	_targetCamera->SetPosition(_initialPosition + _totalMoveAmount );
}

