#ifndef EVENTDATA_HPP
#define EVENTDATA_HPP

#include "../Event/EventsDef.h"

class EventData
{
public:
	EventData(const EventType type, float timestamp = 0.0f ):
	  _eventType(type),
	  _eventTime(timestamp)
	{}
	virtual ~EventData()
	{}

	const EventType& GetEventType() const { return _eventType; } 
	float GetEventTimeStamp() const {return _eventTime; }

protected:
	const float _eventTime;
	const EventType _eventType;
};

#endif