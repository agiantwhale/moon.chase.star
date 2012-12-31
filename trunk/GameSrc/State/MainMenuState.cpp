#include "MainMenuState.hpp"
#include "../App/Game.hpp"
#include "../System/GUIManager.hpp"

MainMenuState::MainMenuState() :	IState(),
									IEventListener("MainMenuState"),
									_menuWindow(nullptr),
									_endState(false)
{
	_menuWindow = new MainMenuControl(GUIManager::GetInstance()->GetCanvas());
	_menuWindow->Hide();

	AddEventListenType(Event_NewGame);
}

MainMenuState::~MainMenuState()
{
	delete _menuWindow;
}

void MainMenuState::Enter( void )
{
	_menuWindow->Show();
}

bool MainMenuState::Update( float deltaTime )
{
	return _endState;
}

void MainMenuState::Render( void )
{
	GUIManager::GetInstance()->Render();
}

void MainMenuState::Exit( void )
{
	_menuWindow->Hide();
}

void MainMenuState::HandleAppEvent( sf::Event& appEvent )
{
	GUIManager::GetInstance()->FeedEvent(appEvent);
}

bool MainMenuState::HandleEvent( const EventData& theevent )
{
	if(theevent.GetEventType() == Event_NewGame)
	{
		Game::GetInstance()->SetNextStateType(State_Loading);
		_endState = true;
	}

	return false;
}

