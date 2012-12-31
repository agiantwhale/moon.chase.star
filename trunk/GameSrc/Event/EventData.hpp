#ifndef EVENTDATA_HPP
#define EVENTDATA_HPP

#include "../Event/EventsDef.h"

class EventData
{
public:
    EventData(const EventType type );
    virtual ~EventData();

    inline const EventType& GetEventType() const
    {
        return _eventType;
    }
    inline float GetEventCreateTimeStamp() const
    {
        return _createTime;
    }
    inline float GetEventStartTimeStamp() const
    {
        return _startTime;
    }

	void TriggerEvent(void);
	void QueueEvent(float waitTime = 0.0f);
	void AbortEvent(bool allOfType = false);

    void StartEvent(void);

protected:
    const float _createTime;	//Timestamp when event was created.
    float _startTime;		//Timestamp when event was started.
    const EventType _eventType;
};

#endif