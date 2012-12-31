#include "../State/PauseState.hpp"
#include "../System/GUIManager.hpp"
#include "../System/GraphicsManager.hpp"
#include "../System/PhysicsManager.hpp"

PauseState::PauseState() :	IState(),
							IEventListener("PauseState"),
							_endState(false)
{

}

PauseState::~PauseState()
{

}

bool PauseState::HandleEvent( const EventData& theevent )
{

}

void PauseState::Enter( void )
{
	_endState = false;
}

bool PauseState::Update( float deltaTime )
{
	return _endState;
}

void PauseState::Render( void )
{
	GraphicsManager::GetInstance()->Render();
	GUIManager::GetInstance()->Render();
}

void PauseState::Exit( void )
{
	_endState = false;
}

