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
    EventData* event;
    float wait;
};

class EventManager : public Singleton<EventManager>
{
    DEFINE_SINGLETON( EventManager )

private:
    typedef std::list<IEventListener*> EventListenerList;
    typedef std::unordered_map<EventType, EventListenerList*> EventListenersMap;
    typedef std::list<EventData*> EventsList;

public:
    void AddListener(IEventListener* const listenerptr, const EventType& eventtype);
    void RemoveListener(IEventListener* const listenerptr, const EventType& eventtype);
    void TriggerEvent(EventData* newevent);
    //************************************
    // Method:    QueueEvent
    // FullName:  EventManager::QueueEvent
    // Access:    public
    // Returns:   void
    // Qualifier:
    // Parameter: EventData * newevent
    // Parameter: float waitTime
    // Parameter: bool isContinuous
    // Parameter: EventData * endEvent
    //************************************
    void QueueEvent(EventData* newevent, float waitTime = 0.0f, bool isContinuous = false, EventData* endEvent = nullptr);
    void AbortEvent(const EventType& typeToAbort, bool alloftype);
    void Update( float dt);
    void EmptyEventQueues();

private:
    EventListenersMap _eventListnersMap;
    std::list<EventQueue> _eventsQueue;
};

#endif