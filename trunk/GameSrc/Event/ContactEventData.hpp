#ifndef ContactEventData_h__
#define ContactEventData_h__

#include <Box2D/Box2D.h>
#include "EventData.hpp"

namespace sb
{
	class ContactEventData : public EventData
	{
	public:
		ContactEventData(b2Contact* contact,EventType eventType) : EventData(eventType), _contact( contact )
		{

		}

		b2Contact* getContact() const { return _contact; }

	private:
		b2Contact* _contact;
	};
}

#endif // ContactEventData_h__
