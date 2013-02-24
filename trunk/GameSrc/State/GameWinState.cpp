#include "GameWinState.hpp"
#include "..\Event\GUIEventData.h"
#include "..\System\InputManager.hpp"
#include "..\GUI\GUIManager.hpp"
#include "..\App\Game.hpp"
#include "..\System\GraphicsManager.hpp"
#include "..\System\SceneManager.hpp"
#include "..\Physics\PhysicsManager.hpp"
#include "..\Event\EventManager.hpp"
#include "..\Entity\EntityManager.hpp"
#include "..\Task\TaskManager.hpp"

sb::GameWinState::GameWinState()
	: m_endState(false),
	  m_gameWinMenuControl(nullptr)
{
	addEventListenType(Event_GUI);
	addEventListenType(Event_App);

	m_gameWinMenuControl = new GameWinMenuControl(sb::GUIManager::getInstance()->getCanvas());
	m_gameWinMenuControl->Hide();
}

sb::GameWinState::~GameWinState()
{
	removeEventListenType(Event_GUI);
	removeEventListenType(Event_App);
}

bool sb::GameWinState::handleEvent( const EventData& theevent )
{
	if( theevent.getEventType() == Event_GUI && isActive() )
	{
		const GUIEventData& eventData = static_cast<const GUIEventData&>(theevent);
		std::string controlName = eventData.getControl()->GetName();

		if( controlName == "NextLevelButton")
		{
			Game::getInstance()->setNextStateType(State_Loading);

			EventData* event = new EventData(Event_NextLevel);
			event->triggerEvent();

			m_endState = true;

		}

		if( controlName == "RestartButton")
		{
			Game::getInstance()->setNextStateType(State_Loading);

			EventData* event = new EventData(Event_RestartLevel);
			event->triggerEvent();

			m_endState = true;

		}

		if( controlName == "MainMenuButton")
		{
			EventData* eventData = new EventData(Event_GameLost);
			eventData->triggerEvent();

			Game::getInstance()->setNextStateType(State_Loading);
			m_endState = true;
		}
	}

	return false;
}

void sb::GameWinState::enter( void )
{
	sb::GameState::enter();

	m_endState = false;
	m_gameWinMenuControl->setLevelName(SceneManager::getInstance()->getSceneName());
	m_gameWinMenuControl->Show();
}

bool sb::GameWinState::update( sf::Time deltaTime )
{
	sb::InputManager::getInstance()->update(deltaTime);
	sb::PhysicsManager::getInstance()->fixedUpdate(deltaTime);
	sb::EventManager::getInstance()->update(deltaTime);
	sb::EntityManager::getInstance()->update(deltaTime);
	sb::TaskManager::getInstance()->update(deltaTime);

	return m_endState;
}

void sb::GameWinState::render( void )
{
	GraphicsManager::getInstance()->render();
	GUIManager::getInstance()->render();
}

void sb::GameWinState::exit( void )
{
	sb::GameState::exit();

	m_gameWinMenuControl->Hide();
}

