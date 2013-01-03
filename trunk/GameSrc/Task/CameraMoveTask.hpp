#ifndef CAMERAMOVETASK_HPP
#define CAMERAMOVETASK_HPP

#include "../Base/Vec2D.hpp"
#include "../Task/Task.hpp"

class Camera;

class CameraMoveTask : public Task
{
public:
	CameraMoveTask( float timeDuration, unsigned int renderLayer, const Vec2D& moveSpeed );

	virtual void Start();
	virtual bool DoTask(float deltaTIme);
	virtual void End();

private:
	unsigned int _renderLayer;
	Vec2D _initialPosition;
	Vec2D _totalMoveAmount;
	Vec2D _moveSpeed;
};

#endif