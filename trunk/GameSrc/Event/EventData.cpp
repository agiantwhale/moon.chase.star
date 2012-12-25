#include "../Event/EventData.hpp"
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
