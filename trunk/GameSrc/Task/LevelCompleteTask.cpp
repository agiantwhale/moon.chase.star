#include "../Task/LevelCompleteTask.hpp"
#include "../Event/EventData.hpp"
#include "../Entity/PlayerEntity.hpp"
#include "../Base/Math.hpp"

const float MOVE_SPEED = 3.0f;

float CalculateTravelTime(const Vec2D& initialPosition, const VectorStack& path)
{
	float totalTravelLength = 0.0f;
	Vec2D previousNode = initialPosition;
	for(VectorStack::const_iterator iter = path.begin(); iter != path.end(); iter++)
	{
		Vec2D currentNode = (*iter);
		totalTravelLength += Magnitude(currentNode - previousNode);
		previousNode = currentNode;
	}

	return totalTravelLength/MOVE_SPEED;
}

LevelCompleteTask::LevelCompleteTask(PlayerEntity* playerEntity, const VectorStack& path)
	:	Task(CalculateTravelTime(playerEntity->GetPosition(), path)),
		_playerEntity(playerEntity),
		_path(path),
		_totalTime(0.0f),
		_elapsedTime(0.0f),
		_xSpline(),
		_ySpline(),
		_totalLerpTime(0.0f),
		_lerpTimer(0.0f),
		_currentState(kLevelComplete_Spline)
{
}

void LevelCompleteTask::Start()
{
	Task::Start();

	_playerEntity->GetBallBody().GetBody()->SetActive(false);

	//_totalLerpTime = Magnitude(_playerEntity->GetPosition() - (*_path.begin())) / MOVE_SPEED;
	//_lerpTimer = _totalLerpTime;

	_xSpline.addPoint(_totalTime,_playerEntity->GetPosition().x);
	_ySpline.addPoint(_totalTime,_playerEntity->GetPosition().y);

	Vec2D previousNode = _playerEntity->GetPosition();
	for(VectorStack::const_iterator iter = _path.begin(); iter != _path.end(); iter++)
	{
		Vec2D currentNode = (*iter);

		{
			_totalTime += Magnitude(currentNode - previousNode)/MOVE_SPEED;
			_xSpline.addPoint(_totalTime,currentNode.x);
			_ySpline.addPoint(_totalTime,currentNode.y);
		}

		previousNode = currentNode;
	}
}

bool LevelCompleteTask::DoTask( sf::Time deltaTime )
{
	if(Task::DoTask(deltaTime))
		return true;

	switch(_currentState)
	{
	case kLevelComplete_Lerp:
		{
			if(_lerpTimer > 0.0f)
			{
				_lerpTimer -= deltaTime;
			}
			else
			{
				_currentState = kLevelComplete_Spline;
				break;
			}

			_playerEntity->SetPosition(lerp<Vec2D>(_playerEntity->GetPosition(), (*_path.begin()), (1.0f - _lerpTimer/_totalLerpTime) ));
			break;
		}

	case kLevelComplete_Spline:
		{
			_elapsedTime += deltaTime;

			if(_elapsedTime >= _totalTime)
			{
				_currentState = kLevelComplete_Arrived;
				_elapsedTime = _totalTime;
			}

			_playerEntity->SetPosition(Vec2D(_xSpline(_elapsedTime),_ySpline(_elapsedTime)));

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

void LevelCompleteTask::End()
{
	Task::End();

	EventData* eventData = new EventData(Event_NextLevel);
	eventData->queueEvent(sf::Seconds(0.5f));
}
