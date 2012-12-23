#ifndef CONTACTEVENTDATA_HPP
#define CONTACTEVENTDATA_HPP

#include <Box2D/Box2D.h>
#include "../Event/EventData.hpp"

class ContactEventData : public EventData
{
public:
	ContactEventData(b2Contact* contact,EventType eventType) : EventData(eventType), _contact( contact )
	{

	}

	b2Contact* const GetContact() const { return _contact; }

private:
	b2Contact* const _contact;
};

#endif