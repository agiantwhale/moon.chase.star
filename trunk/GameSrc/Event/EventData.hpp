#ifndef EVENTDATA_HPP
#define EVENTDATA_HPP

#include "../Event/EventsDef.h"
#include "../App/Game.hpp"

class EventData
{
public:
	EventData(const EventType type ):
	  _eventType(type),
	  _createTime(Game::GetInstance()->GetTime()),
	  _startTime(0.0f)
	{}
	virtual ~EventData()
	{}

	inline const EventType& GetEventType() const { return _eventType; } 
	inline float GetEventCreateTimeStamp() const {return _createTime; }
	inline float GetEventStartTimeStamp() const { return _startTime; }

	inline void	 StartEvent(void) { _startTime = Game::GetInstance()->GetTime(); }

protected:
	const float _createTime;	//Timestamp when event was created.
	float _startTime;		//Timestamp when event was started.
	const EventType _eventType;
};

#endif