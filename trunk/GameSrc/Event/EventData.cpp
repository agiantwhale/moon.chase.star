#include "../Event/EventData.hpp"
#include "../System/EventManager.hpp"
#include "../App/Game.hpp"

EventData::EventData( const EventType type ) :
    _eventType(type),
    _createTime(Game::GetInstance()->GetTime()),
    _startTime(0.0f)
{

}

EventData::~EventData()
{

}

void EventData::StartEvent( void )
{
    _startTime = Game::GetInstance()->GetTime();
}

void EventData::TriggerEvent( void )
{
	EventManager::GetInstance()->TriggerEvent(this);
}

void EventData::QueueEvent( float waitTime )
{
	EventManager::GetInstance()->QueueEvent(this,waitTime);
}

void EventData::AbortEvent( bool allOfType /*= false*/ )
{

}
