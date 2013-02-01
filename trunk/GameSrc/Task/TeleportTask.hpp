#ifndef TELEPORTTASK_HPP
#define TELEPORTTASK_HPP

#include "../Base/Vec2D.hpp"
#include "../Task/Task.hpp"
#include "../Entity/PlayerEntity.hpp"
#include "../Entity/TeleportEntity.hpp"

class TeleportTask : public Task
{
public:
	TeleportTask( float taskDuration, PlayerEntity* playerEntity, TeleportEntity* teleportEntity );

	virtual void Start();
	virtual bool DoTask(float deltaTIme);
	virtual void End();

private:
	float		_totalSeconds;
	PlayerEntity*	_playerEntity;
	TeleportEntity* _teleportEntity;
	Vec2D		_movementSpeed;
};

#endif