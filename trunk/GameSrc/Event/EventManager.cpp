#include <CxxTL/tri_logger.hpp>
#include "../Base/Globals.hpp"
#include "../Event/EventData.hpp"
#include "../System/EventManager.hpp"

namespace sb
{
	SINGLETON_CONSTRUCTOR( EventManager )
	{
	}

	SINGLETON_DESTRUCTOR( EventManager )
	{
	}

	void EventManager::addListener(EventListener* const listenerptr, const EventType& eventtype)
	{
		EventListenersMap::const_iterator iter = m_eventListnersMap.find( eventtype );
		if( iter == m_eventListnersMap.end() )
		{
			iter = m_eventListnersMap.insert( std::make_pair( eventtype, new EventListenerList ) ).first;
		}

		iter->second->push_back( listenerptr );
	}

	void EventManager::removeListener(EventListener* const listenerptr, const EventType& eventtype)
	{
		EventListenersMap::const_iterator iter = m_eventListnersMap.find( eventtype );
		if( iter == m_eventListnersMap.end() )
		{
			TRI_LOG_STR("Attempted to remove unregistered event listener.");
			return;
		}

		EventListenerList* listenerList = iter->second;
		listenerList->remove( listenerptr );

		if( listenerList->empty() )
		{
			delete listenerList;
			m_eventListnersMap.erase( iter );
		}
	}

	void EventManager::triggerEvent(EventData* newevent)
	{
		EventListenersMap::const_iterator iter = m_eventListnersMap.find( newevent->getEventType() );
		if( iter == m_eventListnersMap.end() )
		{
			return;
		}

		EventListenerList* listenerList = iter->second;
		for( EventListenerList::iterator listenerIter = listenerList->begin(); listenerIter != listenerList->end(); listenerIter++ )
		{
			EventListener* listener = (*listenerIter);
			if( listener->handleEvent( *newevent ) )
				break;
		}

		delete newevent;
	}

	void EventManager::queueEvent( EventData* newevent, sf::Time waitTime )
	{
		thor::Timer timer;
		timer.restart(waitTime);
		m_eventsQueue.push_back( std::make_pair(timer,newevent) );
	}

	void EventManager::abortEvent(const EventType& typeToAbort, bool alloftype)
	{
		for( std::list<std::pair<thor::Timer,EventData*>>::iterator iter = m_eventsQueue.begin(); iter != m_eventsQueue.end(); )
		{
			if( iter->second->getEventType() ==  typeToAbort )
			{
				delete iter->second;
				iter = m_eventsQueue.erase(iter);

				if( !alloftype )
					break;
			}
			else
			{
				iter++;
			}
		}
	}

	void EventManager::update( sf::Time deltaTime )
	{
		for( std::list<std::pair<thor::Timer,EventData*>>::iterator iter = m_eventsQueue.begin(); iter != m_eventsQueue.end(); )
		{
			if( iter->first.isExpired() )
			{
				triggerEvent( iter->second );
				iter = m_eventsQueue.erase(iter);
			}
			else
			{
				iter++;
			}
		}
	}

	void EventManager::emptyEventQueues()
	{
		for( std::list<std::pair<thor::Timer,EventData*>>::iterator iter = m_eventsQueue.begin(); iter != m_eventsQueue.end(); iter++ )
		{
			delete iter->second;
		}

		m_eventsQueue.clear();
	}
}