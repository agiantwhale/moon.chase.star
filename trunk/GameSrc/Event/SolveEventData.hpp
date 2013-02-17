#ifndef SolveEventData_h__
#define SolveEventData_h__

#include <Box2D/Box2D.h>
#include "../Event/EventData.hpp"

namespace sb
{
	class PreSolveEventData : public EventData
	{
	public:
		PreSolveEventData(b2Contact* contact, const b2Manifold* oldManifold) : EventData(Event_PreSolve), _contact( contact ), _oldManifold(oldManifold)
		{

		}

		b2Contact* GetContact() const { return _contact; }
		const b2Manifold* GetOldManifold() const { return _oldManifold; }

	private:
		b2Contact* const _contact;
		const b2Manifold* const _oldManifold;
	};

	class PostSolveEventData : public EventData
	{
	public:
		PostSolveEventData(b2Contact* contact,const b2ContactImpulse* impulse) : EventData(Event_PostSolve), _contact( contact ), _impulse(impulse)
		{

		}

		b2Contact* GetContact() const { return _contact; }
		const b2ContactImpulse* GetContactImpulse() const { return _impulse;}

	private:
		b2Contact* const _contact;
		const b2ContactImpulse* const _impulse;
	};
}


#endif // SolveEventData_h__
