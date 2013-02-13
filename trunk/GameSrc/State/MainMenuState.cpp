#include "MainMenuState.hpp"
#include "../App/Game.hpp"
#include "../System/GUIManager.hpp"

MainMenuState::MainMenuState() :	GameState(),
									IEventListener("MainMenuState"),
									_menuWindow(nullptr),
									_endState(false)
{
	_menuWindow = new MainMenuControl(GUIManager::getInstance()->GetCanvas());
	_menuWindow->Hide();

	AddEventListenType(Event_NewGame);
}

MainMenuState::~MainMenuState()
{
	RemoveEventListenType(Event_NewGame);

	delete _menuWindow;
}

void MainMenuState::enter( void )
{
	GameState::enter();

	_menuWindow->Show();
}

bool MainMenuState::Update( float deltaTime )
{
	return _endState;
}

void MainMenuState::Render( void )
{
	GUIManager::getInstance()->Render();
}

void MainMenuState::Exit( void )
{
	GameState::Exit();

	_menuWindow->Hide();
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

