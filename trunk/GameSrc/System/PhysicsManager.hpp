#ifndef PHYSICSMANAGER_H
#define PHYSICSMANAGER_H

#include <list>

#include "../Base/Singleton.hpp"

using namespace std;

class DebugDraw;

namespace sb
{
	class Simulatable;

	class PhysicsManager : public Singleton<PhysicsManager>, private b2ContactListener
	{
		DEFINE_SINGLETON( PhysicsManager )

	public:
		void addSimulatable( Simulatable* simulatable );
		void removeSimulatable( Simulatable* simulatable );

		void setUpPhysics( void );
		void fixedUpdate( float deltaTime );
		void renderPhysicsDebug( void );

		inline b2World* getPhysicsWorld( void ) const
		{
			return m_physicsWorld;
		}

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

		float m_remainderDT;
		float m_remainderRatio;
	};
}

#endif
