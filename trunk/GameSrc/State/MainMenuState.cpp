#include "MainMenuState.hpp"
#include "../App/Game.hpp"
#include "../System/GUIManager.hpp"

MainMenuState::MainMenuState() :	GameState(),
									EventListener("MainMenuState"),
									_menuWindow(nullptr),
									_endState(false)
{
	_menuWindow = new MainMenuControl(GUIManager::getInstance()->getCanvas());
	_menuWindow->Hide();

	addEventListenType(Event_NewGame);
}

MainMenuState::~MainMenuState()
{
	removeEventListenType(Event_NewGame);

	delete _menuWindow;
}

void MainMenuState::enter( void )
{
	GameState::enter();

	_menuWindow->Show();
}

bool MainMenuState::update( sf::Time deltaTime )
{
	return _endState;
}

void MainMenuState::render( void )
{
	GUIManager::getInstance()->render();
}

void MainMenuState::exit( void )
{
	GameState::exit();

	_menuWindow->Hide();
}

bool MainMenuState::handleEvent( const EventData& theevent )
{
	if(theevent.getEventType() == Event_NewGame)
	{
		Game::GetInstance()->SetNextStateType(State_Loading);
		_endState = true;
	}

	return false;
}

