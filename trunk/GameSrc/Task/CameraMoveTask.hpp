#ifndef CAMERAMOVETASK_HPP
#define CAMERAMOVETASK_HPP

#include "../Base/Vec2D.hpp"
#include "../Task/Task.hpp"

class Camera;

class CameraMoveTask : public Task
{
public:
	CameraMoveTask( const Vec2D& moveDistance, unsigned int renderLayer, float affector );

	virtual void Start();
	virtual bool DoTask(float deltaTime);
	virtual void End();

private:
	unsigned int _renderLayer;
	Vec2D _initialPosition;
	Vec2D _finalDestination;
	Vec2D _moveSpeed;
	float _affector;
};

#endif