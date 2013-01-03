#include <CxxTL/tri_logger.hpp>
#include "../App/Game.hpp"
#include "../System/PhysicsManager.hpp"
#include "../System/Debug/PhysicsDebugDraw.hpp"
#include "../System/EventManager.hpp"
#include "../Event/ContactEventData.h"
#include "../Event/SolveEventData.h"

SINGLETON_CONSTRUCTOR( PhysicsManager ),
                       _physicsWorld( nullptr ),
                       _isPhysicsSetUp( false ),
                       _remainderDT( 0.0f ),
                       _remainderRatio( 0.0f ),
                       _debugDraw(nullptr)
{
    _physicsList.clear();
}

SINGLETON_DESTRUCTOR( PhysicsManager )
{
	delete _debugDraw;
	delete _physicsWorld;
}

void PhysicsManager::AddPhysicsWrapper( IPhysics *physics )
{
    if( physics == nullptr ) return;

    _physicsList.push_back( physics );
}

void PhysicsManager::RemovePhysicsWrapper( IPhysics *physics )
{
    if( physics == nullptr ) return;

    _physicsList.remove( physics );
}

void PhysicsManager::SetUpPhysics( void )
{
	_debugDraw = new DebugDraw(*Game::GetInstance());
	_debugDraw->SetFlags(b2Draw::e_shapeBit);

    _physicsWorld = new b2World( b2Vec2( 0, -10 ) );
    _physicsWorld->SetContactListener(this);
    _physicsWorld->SetDebugDraw(_debugDraw);

    _isPhysicsSetUp = true;

	TRI_LOG_STR("Physics initialized.");
}

void PhysicsManager::FixedUpdate( float deltaTime )
{
    if( !_isPhysicsSetUp )
    {
        TRI_LOG_STR("Physics isn't set up!");
        return;
    }

    const float physicsDT = 1.0f/60.0f;
    const int maxSteps = 5;

    _remainderDT += deltaTime;
    const int stepCount = static_cast<int>( floor(_remainderDT/physicsDT) );

    if( stepCount > 0 )
    {
        _remainderDT -= stepCount * physicsDT;
    }

    _remainderRatio = _remainderDT / physicsDT;

    const int stepsClamped = min<int>(stepCount, maxSteps);
    for (int i = 0; i < stepsClamped; ++ i)
    {
        SharpStep();
        SingleStep();
    }

    _physicsWorld->ClearForces();

    SmoothStep();
}

void PhysicsManager::Render(void)
{
    _physicsWorld->DrawDebugData();
}

void PhysicsManager::SingleStep( void )
{
    const float physicsDT = 1.0f/60.0f;
    /*
    for( PhysicsList::iterator iter = _physicsList.begin();
    	iter != _physicsList.end(); iter++ )
    {
    	IPhysics* physics = *iter;
    	physics->Simulate();
    }
    */

    EventManager::GetInstance()->TriggerEvent(new EventData(Event_Simulate));

    _physicsWorld->Step( physicsDT, 10, 10);
}

void PhysicsManager::SmoothStep( void )
{
    for( PhysicsList::iterator iter = _physicsList.begin();
            iter != _physicsList.end(); iter++ )
    {
        IPhysics* physics = *iter;
        physics->SmoothenTransform( _remainderRatio );
        physics->UpdateTransform();
    }
}

void PhysicsManager::SharpStep( void )
{
    for( PhysicsList::iterator iter = _physicsList.begin();
            iter != _physicsList.end(); iter++ )
    {
        IPhysics* physics = *iter;
        physics->ResetTransform();
    }
}

void PhysicsManager::BeginContact(b2Contact* contact)
{
	EventData* eventData = new ContactEventData(contact,Event_BeginContact);
	eventData->TriggerEvent();
}

void PhysicsManager::EndContact(b2Contact* contact)
{
	EventData* eventData = new ContactEventData(contact,Event_EndContact);
	eventData->TriggerEvent();
}

void PhysicsManager::PreSolve( b2Contact* contact, const b2Manifold* oldManifold )
{
	EventData* eventData = new PreSolveEventData(contact,oldManifold);
	eventData->TriggerEvent();
}

void PhysicsManager::PostSolve( b2Contact* contact, const b2ContactImpulse* impulse )
{
	EventData* eventData = new PostSolveEventData(contact,impulse);
	eventData->TriggerEvent();
}
