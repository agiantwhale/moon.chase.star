#include <CxxTL/tri_logger.hpp>
#include "../App/Game.hpp"
#include "Simulatable.hpp"
#include "PhysicsManager.hpp"
#include "Debug/PhysicsDebugDraw.hpp"
#include "../Event/EventManager.hpp"
#include "../Event/ContactEventData.hpp"
#include "../Event/SolveEventData.hpp"
#include "../Event/GravityChangeEventData.hpp"

namespace sb
{
	SINGLETON_CONSTRUCTOR( PhysicsManager ),
		m_physicsWorld( nullptr ),
		m_isPhysicsSetUp( false ),
		m_shouldDrawDebug( false ),
		m_remainderDT(),
		m_remainderRatio( 0.0f ),
		m_debugDraw(nullptr),
		m_gravityDirection(Gravity_Down)
	{
		addEventListenType(Event_NextLevel);
		addEventListenType(Event_RestartLevel);
		addEventListenType(Event_NewGame);
		addEventListenType(Event_GravityChange);
		m_simulatableList.clear();
	}

	SINGLETON_DESTRUCTOR( PhysicsManager )
	{
		removeEventListenType(Event_NextLevel);
		removeEventListenType(Event_RestartLevel);
		removeEventListenType(Event_NewGame);
		removeEventListenType(Event_GravityChange);
		delete m_debugDraw;
		delete m_physicsWorld;
	}

	void PhysicsManager::setUpPhysics( const TiXmlElement* element )
	{
		m_debugDraw = new DebugDraw(*Game::getInstance());
		m_debugDraw->SetFlags(b2Draw::e_shapeBit);

		m_physicsWorld = new b2World( b2Vec2( 0, -GRAVITY_ACCELERATION ) );
		m_physicsWorld->SetAutoClearForces(false);
		m_physicsWorld->SetContactListener(this);
		m_physicsWorld->SetDebugDraw(m_debugDraw);

		m_isPhysicsSetUp = true;

		if(element)
		{
			element->QueryBoolAttribute("Debug",&m_shouldDrawDebug);
		}
		

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
		const int stepCount = static_cast<int>( floor(m_remainderDT.asSeconds()/physicsDT) );

		if( stepCount > 0 )
		{
			m_remainderDT -= sf::seconds(stepCount * physicsDT);
		}

		m_remainderRatio = m_remainderDT.asSeconds() / physicsDT;

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
		if(m_shouldDrawDebug)
		{
			m_physicsWorld->DrawDebugData();
		}
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
			simulatable->resetTransform();
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

	bool PhysicsManager::handleEvent( const EventData& theevent )
	{
		if(theevent.getEventType() == Event_GravityChange)
		{
			const GravityChangeEventData& gravityEvent = static_cast<const GravityChangeEventData&>(theevent);
			setGravityDirection(gravityEvent.getGravityDirection());
		}

		if(theevent.getEventType() == Event_RestartLevel || theevent.getEventType() == Event_NewGame || theevent.getEventType() == Event_NextLevel)
		{
			setGravityDirection(Gravity_Down);
		}

		return false;
	}

	void PhysicsManager::setGravityDirection( GravityDirection val )
	{
		m_gravityDirection = val;
		b2Vec2 gravityDirection;
		gravityDirection.SetZero();

		switch(val)
		{
		case Gravity_Down:
			{
				gravityDirection.y = -GRAVITY_ACCELERATION;
				break;
			}

		case Gravity_Up:
			{
				gravityDirection.y = GRAVITY_ACCELERATION;
				break;
			}

		case Gravity_Left:
			{
				gravityDirection.x = -GRAVITY_ACCELERATION;
				break;
			}

		case Gravity_Right:
			{
				gravityDirection.x = GRAVITY_ACCELERATION;
				break;
			}
		}

		m_physicsWorld->SetGravity(gravityDirection);
	}

}
