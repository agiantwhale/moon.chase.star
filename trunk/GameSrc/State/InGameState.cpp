#include "InGameState.hpp"
#include "../App/Game.hpp"
#include "../System/GraphicsManager.hpp"
#include "../Entity/EntityManager.hpp"
#include "../Physics/PhysicsManager.hpp"
#include "../System/SceneManager.hpp"
#include "../Event/EventManager.hpp"
#include "../Task/TaskManager.hpp"
#include "../system/InputManager.hpp"
#include "../Event/AppEventData.hpp"

namespace sb
{
	InGameState::InGameState() :	GameState(),
									EventListener(),
									m_endState(false)
	{
		addEventListenType(Event_RestartLevel);
		addEventListenType(Event_NextLevel);
		addEventListenType(Event_App);
	}

	InGameState::~InGameState()
	{
		removeEventListenType(Event_RestartLevel);
		removeEventListenType(Event_NextLevel);
		removeEventListenType(Event_App);
	}

	void InGameState::enter()
	{
		GameState::enter();

		sb::Game::getInstance()->setMouseCursorVisible(false);

		m_endState = false;
	}

	void InGameState::render()
	{
		sb::GraphicsManager::getInstance()->render();
		sb::PhysicsManager::getInstance()->renderPhysicsDebug();
	}

	bool InGameState::update(sf::Time deltaTime)
	{
		sb::InputManager::getInstance()->update(deltaTime);
		sb::PhysicsManager::getInstance()->fixedUpdate(deltaTime);
		sb::EventManager::getInstance()->update(deltaTime);
		sb::EntityManager::getInstance()->update(deltaTime);
		sb::TaskManager::getInstance()->update(deltaTime);

		return m_endState;
	}

	void InGameState::exit()
	{
		GameState::exit();

		sb::Game::getInstance()->setMouseCursorVisible(true);

		m_endState = false;
	}

	bool InGameState::handleEvent( const EventData& theevent )
	{
		if((theevent.getEventType() == Event_NextLevel || theevent.getEventType() == Event_RestartLevel) && isActive())
		{
			Game::getInstance()->setNextStateType(sb::State_Loading);
			m_endState = true;
		}

		if(theevent.getEventType()==Event_App && isActive())
		{
			const AppEventData& eventData = static_cast<const AppEventData&>(theevent);

			if(eventData.getAppEvent().type == sf::Event::KeyReleased &&
				eventData.getAppEvent().key.code == sf::Keyboard::Escape )
			{
				sb::Game::getInstance()->setNextStateType(sb::State_Paused);
				m_endState = true;
			}
		}

		return false;
	}

}