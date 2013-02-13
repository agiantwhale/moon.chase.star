#include "../State/PauseState.hpp"
#include "../System/GUIManager.hpp"
#include "../System/GraphicsManager.hpp"
#include "../System/PhysicsManager.hpp"
#include "../System/InputManager.hpp"
#include "../Event/GUIEventData.h"
#include "../App/Game.hpp"
#include "../Event/AppEventData.hpp"

PauseState::PauseState() :	GameState(),
							EventListener("PauseState"),
							_pauseMenuControl(nullptr),
							_endState(false)
{
	addEventListenType(Event_GUI);
	addEventListenType(Event_App);

	_pauseMenuControl = new PauseMenuControl(GUIManager::getInstance()->GetCanvas());
	_pauseMenuControl->Hide();
}

PauseState::~PauseState()
{
	removeEventListenType(Event_GUI);
	removeEventListenType(Event_App);
}

bool PauseState::handleEvent( const EventData& theevent )
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

	if(theevent.GetEventType()==Event_App && isActive())
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

void PauseState::enter( void )
{
	GameState::enter();

	_endState = false;
	_pauseMenuControl->Show();
}

bool PauseState::update( float deltaTime )
{
	InputManager::getInstance()->Update( deltaTime );

	return _endState;
}

void PauseState::render( void )
{
	GraphicsManager::getInstance()->Render();
	GUIManager::getInstance()->Render();
}

void PauseState::exit( void )
{
	GameState::exit();

	_endState = false;
	_pauseMenuControl->Hide();
}

