#ifndef LEVELCOMPLETETASK_HPP
#define LEVELCOMPLETETASK_HPP

#include <vector>
#include "../Task/Task.hpp"
#include "../Base/Vec2D.hpp"
#include "../Base/Math.hpp"

typedef std::vector<Vec2D> VectorStack;

class PlayerEntity;

class LevelCompleteTask : public Task
{
public:
	LevelCompleteTask(PlayerEntity* playerEntity, const VectorStack& path);

	virtual void Start();
	virtual bool DoTask(float deltaTime);
	virtual void End();

private:
	PlayerEntity* _playerEntity;
	VectorStack _path;

	float _totalTime;
	float _elapsedTime;
	Spline _xSpline;
	Spline _ySpline;

	float _totalLerpTime;
	float _lerpTimer;

	enum
	{
		kLevelComplete_Lerp,
		kLevelComplete_Spline,
		kLevelComplete_Arrived
	} _currentState;
};

#endif