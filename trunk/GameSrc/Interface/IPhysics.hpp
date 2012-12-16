#ifndef IPHYSICS_H
#define IPHYSICS_H

#include <Box2D\Box2D.h>

#include "../Entity/Entity.hpp"

/*
    IPhysics
    -Interface for physics entities
    -Also takes care of interpolation
*/

class IPhysics
{
public:
	IPhysics(Entity* const entity);
	virtual ~IPhysics();

	virtual void Simulate( void ) = 0;
	virtual void UpdateTransform( void ) = 0;
	virtual void SmoothenTransform( float remainderRatio ) = 0;
	virtual void ResetTransform( void ) = 0;

	virtual void BeginContact(b2Contact* contact, const b2Fixture* contactFixture ) = 0;
	virtual void EndContact(b2Contact* contact, const b2Fixture* contactFixture ) = 0;

    inline Entity* const GetEntity(void) const {return _entity;}

private:
    Entity* const _entity;
};

inline IPhysics *GetPhysicsInterface(const b2Fixture *fixture)
{
    void* bodyData = fixture->GetBody()->GetUserData();
    if(bodyData != nullptr)
    {
       return static_cast<IPhysics*>(bodyData);
    }

    return nullptr;
}

#endif
