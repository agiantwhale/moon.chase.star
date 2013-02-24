#include "../State/PauseState.hpp"
#include "../GUI/GUIManager.hpp"
#include "../System/GraphicsManager.hpp"
#include "../Physics/PhysicsManager.hpp"
#include "../System/InputManager.hpp"
#include "../Event/GUIEventData.h"
#include "../App/Game.hpp"
#include "../Event/AppEventData.hpp"

namespace sb
{
	PauseState::PauseState() :	GameState(),
		EventListener(),
		m_pauseMenuControl(nullptr),
		m_endState(false)
	{
		addEventListenType(Event_RestartLevel);
		addEventListenType(Event_GUI);
		addEventListenType(Event_App);

		m_pauseMenuControl = new PauseMenuControl(GUIManager::getInstance()->getCanvas());
		m_pauseMenuControl->Hide();
	}

	PauseState::~PauseState()
	{
		removeEventListenType(Event_RestartLevel);
		removeEventListenType(Event_GUI);
		removeEventListenType(Event_App);
	}

	bool PauseState::handleEvent( const EventData& theevent )
	{
		if( theevent.getEventType() == Event_RestartLevel && isActive() )
		{
			Game::getInstance()->setNextStateType(sb::State_Loading);
			m_endState = true;
		}

		if( theevent.getEventType() == Event_GUI)
		{
			const GUIEventData& eventData = static_cast<const GUIEventData&>(theevent);
			std::string controlName = eventData.getControl()->GetName();

			if( controlName == "ResumeButton")
			{
				Game::getInstance()->setNextStateType(State_InGame);

				EventData* event = new EventData(Event_ResumeGame);
				event->triggerEvent();

				m_endState = true;

			}

			if( controlName == "RestartButton")
			{
				//Game::getInstance()->setNextStateType(State_Loading);

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

		if(theevent.getEventType()==Event_App && isActive())
		{
			const AppEventData& eventData = static_cast<const AppEventData&>(theevent);

			if(eventData.getAppEvent().type == sf::Event::KeyReleased &&
				eventData.getAppEvent().key.code == sf::Keyboard::Escape )
			{
				Game::getInstance()->setNextStateType(State_InGame);

				EventData* event = new EventData(Event_ResumeGame);
				event->triggerEvent();

				m_endState = true;
			}
		}

		return false;
	}

	void PauseState::enter( void )
	{
		GameState::enter();

		m_endState = false;
		m_pauseMenuControl->Show();

		EventData* event = new EventData(Event_PauseGame);
		event->triggerEvent();
	}

	bool PauseState::update( sf::Time deltaTime )
	{
		InputManager::getInstance()->update( deltaTime );

		return m_endState;
	}

	void PauseState::render( void )
	{
		GraphicsManager::getInstance()->render();
		GUIManager::getInstance()->render();
	}

	void PauseState::exit( void )
	{
		GameState::exit();

		m_endState = false;
		m_pauseMenuControl->Hide();
	}
}