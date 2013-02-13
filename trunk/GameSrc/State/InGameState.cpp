#include "../State/InGameState.hpp"
#include "../App/Game.hpp"
#include "../System/GraphicsManager.hpp"
#include "../System/EntityManager.hpp"
#include "../System/PhysicsManager.hpp"
#include "../System/SceneManager.hpp"
#include "../System/EventManager.hpp"
#include "../System/TaskManager.hpp"
#include "../system/InputManager.hpp"
#include "../Event/AppEventData.hpp"

InGameState::InGameState() :	GameState(),
								EventListener("InGameState"),
								_endState(false)
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

	Game::GetInstance()->setMouseCursorVisible(false);

	_endState = false;
}

void InGameState::render()
{
    GraphicsManager::getInstance()->Render();
    //PhysicsManager::GetInstance()->Render();
}

bool InGameState::update(float deltaTime)
{
	InputManager::getInstance()->Update( deltaTime );
    PhysicsManager::GetInstance()->FixedUpdate( deltaTime );
	EventManager::GetInstance()->Update( deltaTime );
    EntityManager::getInstance()->Update( deltaTime );
	TaskManager::getInstance()->Update( deltaTime );

    return _endState;
}

void InGameState::exit()
{
	GameState::exit();

	Game::GetInstance()->setMouseCursorVisible(true);

	_endState = false;
}

bool InGameState::handleEvent( const EventData& theevent )
{
	if((theevent.GetEventType() == Event_NextLevel || theevent.GetEventType() == Event_RestartLevel) && isActive())
	{
		Game::GetInstance()->SetNextStateType(State_Loading);
		_endState = true;
	}

	if(theevent.GetEventType()==Event_App && isActive())
	{
		const AppEventData& eventData = static_cast<const AppEventData&>(theevent);

		if(eventData.GetAppEvent().type == sf::Event::KeyReleased &&
			eventData.GetAppEvent().key.code == sf::Keyboard::Escape )
		{
			Game::GetInstance()->SetNextStateType(State_Paused);
			_endState = true;
		}
	}

	return false;
}
