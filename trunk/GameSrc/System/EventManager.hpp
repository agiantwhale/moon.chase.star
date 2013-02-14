#ifndef EventManager_h__
#define EventManager_h__

#include <list>
#include <unordered_map>
#include <queue>
#include <SFML/System/Time.hpp>
#include <Thor/Time/Timer.hpp>
#include "../Base/Singleton.hpp"
#include "../Event/EventListener.hpp"
#include "../Event/EventData.hpp"

namespace sb
{
	class EventManager : public Singleton<EventManager>
	{
		DEFINE_SINGLETON( EventManager )

	private:
		typedef std::list<EventListener*> EventListenerList;
		typedef std::unordered_map<EventType, EventListenerList*> EventListenersMap;
		typedef std::list<EventData*> EventsList;

	public:
		void addListener(EventListener* const listenerptr, const EventType& eventtype);
		void removeListener(EventListener* const listenerptr, const EventType& eventtype);
		void triggerEvent(EventData* newevent);
		void queueEvent(EventData* newevent, sf::Time waitTime = sf::Time::Zero );
		void abortEvent(const EventType& typeToAbort, bool alloftype);
		void update( sf::Time deltaTime );
		void emptyEventQueues();

	private:
		EventListenersMap m_eventListnersMap;
		std::list<std::pair<thor::Timer,EventData*>> m_eventsQueue;
	};
}

#endif // EventManager_h__
