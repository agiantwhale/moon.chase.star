#include "../Interface/IEventListener.hpp"
#include "../System/EventManager.hpp"

void IEventListener::AddEventListenType( const EventType& eventType )
{
	EventManager::GetInstance()->AddListener(this,eventType);
}

void IEventListener::RemoveEventListenType( const EventType& eventType )
{
	EventManager::GetInstance()->RemoveListener(this,eventType);
}