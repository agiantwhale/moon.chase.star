#include <CxxTL/tri_logger.hpp>
#include "../Base/Globals.hpp"
#include "../Event/EventData.hpp"
#include "../System/EventManager.hpp"

SINGLETON_CONSTRUCTOR( EventManager )
{
}

SINGLETON_DESTRUCTOR( EventManager )
{
}

void EventManager::AddListener(IEventListener* const listenerptr, const EventType& eventtype)
{
    EventListenersMap::const_iterator iter = _eventListnersMap.find( eventtype );
    if( iter == _eventListnersMap.end() )
    {
        iter = _eventListnersMap.insert( std::make_pair( eventtype, new EventListenerList ) ).first;
    }

    iter->second->push_back( listenerptr );
}

void EventManager::RemoveListener(IEventListener* const listenerptr, const EventType& eventtype)
{
    EventListenersMap::const_iterator iter = _eventListnersMap.find( eventtype );
    if( iter == _eventListnersMap.end() )
    {
		TRI_LOG_STR("Attempted to remove unregistered event listener.");
		TRI_LOG(listenerptr->GetHandlerName());
        return;
    }

    EventListenerList* listenerList = iter->second;
    listenerList->remove( listenerptr );

    if( listenerList->empty() )
    {
        delete listenerList;
        _eventListnersMap.erase( iter );
    }
}

void EventManager::TriggerEvent(EventData* newevent)
{
    EventListenersMap::const_iterator iter = _eventListnersMap.find( newevent->GetEventType() );
    if( iter == _eventListnersMap.end() )
    {
        return;
    }

    newevent->StartEvent();

    EventListenerList* listenerList = iter->second;
    for( EventListenerList::iterator listenerIter = listenerList->begin(); listenerIter != listenerList->end(); listenerIter++ )
    {
        IEventListener* listener = (*listenerIter);
        if( listener->HandleEvent( *newevent ) )
            break;
    }

    delete newevent;
}

void EventManager::QueueEvent( EventData* newevent, float waitTime /*= 0.0f*/ )
{
    EventQueue eventQueue;
    eventQueue.event = newevent;
    eventQueue.wait = waitTime;
    _eventsQueue.push_back( eventQueue );
}

void EventManager::AbortEvent(const EventType& typeToAbort, bool alloftype)
{
    for( std::list<EventQueue>::iterator iter = _eventsQueue.begin(); iter != _eventsQueue.end(); )
    {
        EventQueue& queue = (*iter);
        if( queue.event->GetEventType() ==  typeToAbort )
        {
            delete queue.event;
            iter = _eventsQueue.erase(iter);

            if( !alloftype )
                break;
        }
        else
        {
            iter++;
        }
    }
}

void EventManager::Update( float dt)
{
    for( std::list<EventQueue>::iterator iter = _eventsQueue.begin(); iter != _eventsQueue.end(); )
    {
        EventQueue& queue = (*iter);
        queue.wait -= dt;

        if( queue.wait <= 0 )
        {
            TriggerEvent( queue.event );
            iter = _eventsQueue.erase(iter);
        }
        else
        {
            iter++;
        }
    }
}

void EventManager::EmptyEventQueues()
{
    for( std::list<EventQueue>::iterator iter = _eventsQueue.begin(); iter != _eventsQueue.end(); iter++ )
    {
        EventQueue& queue = (*iter);
        delete queue.event;
    }

    _eventsQueue.clear();
}