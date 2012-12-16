#include "../Interface/IPhysics.hpp"
#include "../System/PhysicsManager.hpp"

IPhysics::IPhysics(Entity* const entity) : _entity(entity)
{
	PhysicsManager::GetInstance()->AddPhysicsWrapper( this );
}

IPhysics::~IPhysics()
{
	PhysicsManager::GetInstance()->RemovePhysicsWrapper( this );
}
