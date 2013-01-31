#ifndef TELEPORTTASK_HPP
#define TELEPORTTASK_HPP

#include "../Base/Vec2D.hpp"
#include "../Task/Task.hpp"
#include "../Interface/ITransform.hpp"
#include "../Entity/Component/BodyWrapper.hpp"

class TeleportTask : public Task
{
public:
	TeleportTask( float taskDuration, ITransform* targetTransform, b2Body* _targetBody, const Vec2D& destination );

	virtual void Start();
	virtual bool DoTask(float deltaTIme);
	virtual void End();

private:
	float		_totalSeconds;
	ITransform* _targetTransform;
	b2Body*		_targetBody;
	Vec2D		_destination;
	Vec2D		_movementSpeed;
};

#endif