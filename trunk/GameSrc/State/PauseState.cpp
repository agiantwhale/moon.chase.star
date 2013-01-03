#include "../State/PauseState.hpp"
#include "../System/GUIManager.hpp"
#include "../System/GraphicsManager.hpp"
#include "../System/PhysicsManager.hpp"
#include "../Event/GUIEventData.h"
#include "../App/Game.hpp"
#include "../Event/AppEventData.hpp"

PauseState::PauseState() :	IState(),
							IEventListener("PauseState"),
							_pauseMenuControl(nullptr),
							_endState(false)
{
	AddEventListenType(Event_GUI);
	AddEventListenType(Event_App);

	_pauseMenuControl = new PauseMenuControl(GUIManager::GetInstance()->GetCanvas());
	_pauseMenuControl->Hide();
}

PauseState::~PauseState()
{

}

bool PauseState::HandleEvent( const EventData& theevent )
{
	if( theevent.GetEventType() == Event_GUI)
	{
		const GUIEventData& eventData = static_cast<const GUIEventData&>(theevent);
		std::string controlName = eventData.GetControl()->GetName();

		if( controlName == "ResumeButton")
		{
			Game::GetInstance()->SetNextStateType(State_InGame);
			_endState = true;

		}

		if( controlName == "MainMenuButton")
		{
			Game::GetInstance()->SetNextStateType(State_Loading);
			_endState = true;
		}
	}

	if(theevent.GetEventType()==Event_App && IsActive())
	{
		const AppEventData& eventData = static_cast<const AppEventData&>(theevent);

		if(eventData.GetAppEvent().type == sf::Event::KeyReleased &&
			eventData.GetAppEvent().key.code == sf::Keyboard::Escape )
		{
			Game::GetInstance()->SetNextStateType(State_InGame);
			_endState = true;
		}
	}

	return false;
}

void PauseState::Enter( void )
{
	IState::Enter();

	_endState = false;
	_pauseMenuControl->Show();
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
	IState::Exit();

	_endState = false;
	_pauseMenuControl->Hide();
}

