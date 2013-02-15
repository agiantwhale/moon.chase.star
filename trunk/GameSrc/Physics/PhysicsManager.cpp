#include <CxxTL/tri_logger.hpp>
#include "../App/Game.hpp"
#include "../Helper/Simulatable.hpp"
#include "../System/PhysicsManager.hpp"
#include "../System/Debug/PhysicsDebugDraw.hpp"
#include "../System/EventManager.hpp"
#include "../Event/ContactEventData.h"
#include "../Event/SolveEventData.h"

namespace sb
{
	SINGLETON_CONSTRUCTOR( PhysicsManager ),
		m_physicsWorld( nullptr ),
		m_isPhysicsSetUp( false ),
		m_remainderDT( 0.0f ),
		m_remainderRatio( 0.0f ),
		m_debugDraw(nullptr)
	{
		m_simulatableList.clear();
	}

	SINGLETON_DESTRUCTOR( PhysicsManager )
	{
		delete m_debugDraw;
		delete m_physicsWorld;
	}

	void PhysicsManager::setUpPhysics( void )
	{
		m_debugDraw = new DebugDraw(*Game::getInstance());
		m_debugDraw->SetFlags(b2Draw::e_shapeBit);

		m_physicsWorld = new b2World( b2Vec2( 0, -GRAVITY_ACCELERATION ) );
		m_physicsWorld->SetAutoClearForces(false);
		m_physicsWorld->SetContactListener(this);
		m_physicsWorld->SetDebugDraw(m_debugDraw);

		m_isPhysicsSetUp = true;

		TRI_LOG_STR("Physics initialized.");
	}

	void PhysicsManager::fixedUpdate( sf::Time deltaTime )
	{
		if( !m_isPhysicsSetUp )
		{
			TRI_LOG_STR("Physics isn't set up!");
			return;
		}

		const float physicsDT = 1.0f/60.0f;
		const int maxSteps = 5;

		m_remainderDT += deltaTime;
		const int stepCount = static_cast<int>( floor(m_remainderDT/physicsDT) );

		if( stepCount > 0 )
		{
			m_remainderDT -= stepCount * physicsDT;
		}

		m_remainderRatio = m_remainderDT / physicsDT;

		const int stepsClamped = min<int>(stepCount, maxSteps);
		for (int i = 0; i < stepsClamped; ++ i)
		{
			sharpStep();
			singleStep();
		}

		m_physicsWorld->ClearForces();

		smoothStep();
	}

	void PhysicsManager::renderPhysicsDebug(void)
	{
		m_physicsWorld->DrawDebugData();
	}

	void PhysicsManager::singleStep( void )
	{
		const float physicsDT = 1.0f/60.0f;

		EventManager::getInstance()->triggerEvent(new EventData(Event_Simulate));

		m_physicsWorld->Step( physicsDT, 8, 3);
	}

	void PhysicsManager::smoothStep( void )
	{
		for( SimulatableList::iterator iter = m_simulatableList.begin();
			iter != m_simulatableList.end(); iter++ )
		{
			Simulatable* simulatable = *iter;
			simulatable->smoothenTransform(m_remainderRatio);
			simulatable->updateTransform();
		}
	}

	void PhysicsManager::sharpStep( void )
	{
		for( SimulatableList::iterator iter = m_simulatableList.begin();
			iter != m_simulatableList.end(); iter++ )
		{
			Simulatable* simulatable = *iter;
			simulatable->updateTransform();
		}
	}

	void PhysicsManager::BeginContact(b2Contact* contact)
	{
		EventData* eventData = new ContactEventData(contact,Event_BeginContact);
		eventData->triggerEvent();
	}

	void PhysicsManager::EndContact(b2Contact* contact)
	{
		EventData* eventData = new ContactEventData(contact,Event_EndContact);
		eventData->triggerEvent();
	}

	void PhysicsManager::PreSolve( b2Contact* contact, const b2Manifold* oldManifold )
	{
		EventData* eventData = new PreSolveEventData(contact,oldManifold);
		eventData->triggerEvent();
	}

	void PhysicsManager::PostSolve( b2Contact* contact, const b2ContactImpulse* impulse )
	{
		EventData* eventData = new PostSolveEventData(contact,impulse);
		eventData->triggerEvent();
	}

	void PhysicsManager::addSimulatable( Simulatable* simulatable )
	{
		if(!simulatable) return;

		m_simulatableList.push_back(simulatable);
	}

	void PhysicsManager::removeSimulatable( Simulatable* simulatable )
	{
		if(!simulatable) return;

		m_simulatableList.remove(simulatable);
	}
}