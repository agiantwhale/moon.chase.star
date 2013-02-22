#ifndef PHYSICSMANAGER_H
#define PHYSICSMANAGER_H

#include <sfml/System/Time.hpp>
#include <list>
#include <Box2D/Box2D.h>
#include "../Base/Singleton.hpp"
#include "../Event/EventListener.hpp"

using namespace std;

class DebugDraw;

namespace sb
{
	class Simulatable;

	class PhysicsManager : public Singleton<PhysicsManager>, private b2ContactListener, private EventListener
	{
		DEFINE_SINGLETON( PhysicsManager )

	public:
		enum GravityDirection
		{
			Gravity_Left,
			Gravity_Up,
			Gravity_Right,
			Gravity_Down
		};

		void addSimulatable( Simulatable* simulatable );
		void removeSimulatable( Simulatable* simulatable );

		void setUpPhysics( void );
		void fixedUpdate( sf::Time deltaTime );
		void renderPhysicsDebug( void );

		inline b2World* getPhysicsWorld( void ) const
		{
			return m_physicsWorld;
		}

		sb::PhysicsManager::GravityDirection getGravityDirection() const { return m_gravityDirection; }
		void setGravityDirection(sb::PhysicsManager::GravityDirection val);

	private:
		virtual void BeginContact(b2Contact* contact);
		virtual void EndContact(b2Contact* contact);
		virtual void PreSolve(b2Contact* contact, const b2Manifold* oldManifold);
		virtual void PostSolve(b2Contact* contact, const b2ContactImpulse* impulse);

		typedef std::list<Simulatable*> SimulatableList;
		SimulatableList m_simulatableList;

		DebugDraw*	m_debugDraw;
		b2World*	m_physicsWorld;
		bool		m_isPhysicsSetUp;

		void singleStep( void );
		void smoothStep( void );
		void sharpStep( void );

		virtual bool handleEvent( const EventData& theevent );

		sf::Time m_remainderDT;
		float m_remainderRatio;
		GravityDirection m_gravityDirection;
	};
}

#endif
