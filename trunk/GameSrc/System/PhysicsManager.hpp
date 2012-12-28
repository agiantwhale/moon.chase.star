#ifndef PHYSICSMANAGER_H
#define PHYSICSMANAGER_H

#include <list>

#include "../Base/Singleton.hpp"
#include "../Interface/IPhysics.hpp"
#include "../System/Debug/PhysicsDebugDraw.hpp"

using namespace std;

class PhysicsManager : public Singleton<PhysicsManager>, public b2ContactListener
{
    DEFINE_SINGLETON( PhysicsManager )

public:
    void AddPhysicsWrapper( IPhysics *physics );
    void RemovePhysicsWrapper( IPhysics *physics );

    void SetUpPhysics( void );
    void FixedUpdate( float deltaTime );
    void Render( void );

    virtual void BeginContact(b2Contact* contact);
    virtual void EndContact(b2Contact* contact);

    inline b2World* GetWorld( void ) const
    {
        return _physicsWorld;
    }
    inline bool IsPhysicsSetUp( void ) const
    {
        return _isPhysicsSetUp;
    }

private:
    typedef std::list<IPhysics*> PhysicsList;
    PhysicsList _physicsList;

    b2World* _physicsWorld;
    bool _isPhysicsSetUp;

    void SingleStep( void );
    void SmoothStep( void );
    void SharpStep( void );

    float _remainderDT;
    float _remainderRatio;

    DebugDraw _debugDraw;
};

#endif
