#include "EventListener.hpp"
#include "EventManager.hpp"

void sb::EventListener::addEventListenType( const EventType& eventType )
{
	EventManager::getInstance()->addListener(this,eventType);
}

void sb::EventListener::removeEventListenType( const EventType& eventType )
{
	EventManager::getInstance()->removeListener(this,eventType);
}
