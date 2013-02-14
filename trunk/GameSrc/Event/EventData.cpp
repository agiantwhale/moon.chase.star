#include "../Event/EventData.hpp"
#include "../System/EventManager.hpp"

namespace sb
{
	EventData::EventData( EventType type ) :
		m_eventType(type)
	{

	}

	EventData::~EventData()
	{

	}

	void EventData::triggerEvent( void )
	{
		EventManager::getInstance()->triggerEvent(this);
	}

	void EventData::queueEvent( sf::Time waitTime /*= sf::Time::Zero */ )
	{
		EventManager::getInstance()->queueEvent(this,waitTime);
	}

	void EventData::abortEvent( bool allOfType /*= false*/ )
	{
		EventManager::getInstance()->abortEvent(m_eventType,allOfType);
	}

}

