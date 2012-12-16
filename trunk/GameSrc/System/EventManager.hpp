#ifndef EVENTMANAGER_HPP
#define EVENTMANAGER_HPP

#include <list>
#include <unordered_map>
#include <queue>

#include "../Base/Singleton.hpp"
#include "../Interface/IEventListner.hpp"
#include "../Event/EventData.hpp"

class EventManager : public Singleton<EventManager>
{
	DEFINE_SINGLETON( EventManager )

private:
	typedef std::list<IEventListener*> EventListenerList;
	typedef std::unordered_map<EventType, EventListenerList> EventListenersMap;
	typedef std::list<const EventData*> EventsList;
	typedef std::pair<EventListenersMap::iterator,bool> EvListInsertResult;
	typedef std::pair<EventData, EventListenerList> EvListEntry;

public:
	void AddListener(IEventListener* const listenerptr, const EventType& eventtype);
	void RemoveListener(IEventListener* const listenerptr, const EventType& eventtype);
	void TriggerEvent(const EventData* newevent);
	void QueueEvent(const EventData* newevent);
	void AbortEvent(const EventType& typeToAbort, bool alloftype);
	void Update( float dt);
	void EmptyEventQueues();

private:
	EventListenersMap _eventListnersMap;
	std::queue<EventsList> _eventsListQueue;
};

#endif