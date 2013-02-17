#include "MainMenuState.hpp"
#include "../App/Game.hpp"
#include "../GUI/GUIManager.hpp"

namespace sb
{
	MainMenuState::MainMenuState() :	GameState(),
		EventListener(),
		m_menuWindow(nullptr),
		m_endState(false)
	{
		m_menuWindow = new MainMenuControl(GUIManager::getInstance()->getCanvas());
		m_menuWindow->Hide();

		addEventListenType(Event_NewGame);
	}

	MainMenuState::~MainMenuState()
	{
		removeEventListenType(Event_NewGame);

		delete m_menuWindow;
	}

	void MainMenuState::enter( void )
	{
		GameState::enter();

		m_menuWindow->Show();
	}

	bool MainMenuState::update( sf::Time deltaTime )
	{
		return m_endState;
	}

	void MainMenuState::render( void )
	{
		GUIManager::getInstance()->render();
	}

	void MainMenuState::exit( void )
	{
		GameState::exit();

		m_menuWindow->Hide();
	}

	bool MainMenuState::handleEvent( const EventData& theevent )
	{
		if(theevent.getEventType() == Event_NewGame)
		{
			Game::getInstance()->setNextStateType(State_Loading);
			m_endState = true;
		}

		return false;
	}

}