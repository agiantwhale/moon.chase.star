#ifndef GravityChangeEvent_h__
#define GravityChangeEvent_h__

#include "EventData.hpp"
#include "../Physics/PhysicsManager.hpp"

class GravityChangeEventData : public sb::EventData
{
public:
	GravityChangeEventData(sb::PhysicsManager::GravityDirection gravityDirection) : sb::EventData(Event_GravityChange), m_gravityDirection(gravityDirection)
	{

	}

	sb::PhysicsManager::GravityDirection getGravityDirection() const { return m_gravityDirection; }
	void setGravityDirection(sb::PhysicsManager::GravityDirection val) { m_gravityDirection = val; }

private:
	sb::PhysicsManager::GravityDirection m_gravityDirection;
};

#endif // GravityChangeEvent_h__
