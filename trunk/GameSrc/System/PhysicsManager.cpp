#include <glog\logging.h>
#include "../App/Game.hpp"
#include "../System/PhysicsManager.hpp"
#include "../System/Debug/PhysicsDebugDraw.hpp"
#include "../System/EventManager.hpp"
#include "../Event/ContactEventData.h"

SINGLETON_CONSTRUCTOR( PhysicsManager ),	_physicsWorld( NULL ),
											_isPhysicsSetUp( false ),
											_remainderDT( 0.0f ),
											_remainderRatio( 0.0f ),
											_debugDraw(*Game::GetInstance())
{
	_physicsList.clear();
	_debugDraw.SetFlags(b2Draw::e_shapeBit);
}

SINGLETON_DESTRUCTOR( PhysicsManager )
{
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
	_physicsWorld = new b2World( b2Vec2( 0, -10 ) );
	_physicsWorld->SetContactListener(this);
	_physicsWorld->SetDebugDraw(&_debugDraw);

	_isPhysicsSetUp = true;

	LOG(INFO) << "Physics initialized.";
}

void PhysicsManager::FixedUpdate( float deltaTime )
{
	if( !_isPhysicsSetUp )
	{
		LOG(ERROR) << "Physics isn't set up!";
		return;
	}

    const float physicsDT = 1.0f/60.0f;
	const int maxSteps = 5;

	_remainderDT += deltaTime;
	const int stepCount = static_cast<int>( floor(_remainderDT/physicsDT) );

	if( stepCount > 0 ) {
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
	EventManager::GetInstance()->TriggerEvent(new ContactEventData(contact,Event_BeginContact));
}

void PhysicsManager::EndContact(b2Contact* contact)
{
    EventManager::GetInstance()->TriggerEvent(new ContactEventData(contact,Event_EndContact));
}
