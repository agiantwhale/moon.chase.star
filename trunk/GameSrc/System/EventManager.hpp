#ifndef EVENTMANAGER_HPP
#define EVENTMANAGER_HPP

#include <list>
#include <unordered_map>
#include <queue>

#include "../Base/Singleton.hpp"
#include "../Interface/IEventListner.hpp"
#include "../Event/EventData.hpp"

//Used internally by EventManager
struct EventQueue
{
	const EventData* event;
	float wait;
};

class EventManager : public Singleton<EventManager>
{
	DEFINE_SINGLETON( EventManager )

private:
	typedef std::list<IEventListener* const> EventListenerList;
	typedef std::unordered_map<EventType, EventListenerList*> EventListenersMap;
	typedef std::list<const EventData*> EventsList;

public:
	void AddListener(IEventListener* const listenerptr, const EventType& eventtype);
	void RemoveListener(IEventListener* const listenerptr, const EventType& eventtype);
	void TriggerEvent(const EventData* newevent);
	void QueueEvent(const EventData* newevent, float waitTime);
	void AbortEvent(const EventType& typeToAbort, bool alloftype);
	void Update( float dt);
	void EmptyEventQueues();

private:
	EventListenersMap _eventListnersMap;
	std::list<EventQueue> _eventsQueue;
};

#endif