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
								IEventListener("InGameState"),
								_endState(false)
{
	AddEventListenType(Event_RestartLevel);
	AddEventListenType(Event_NextLevel);
	AddEventListenType(Event_App);
}

InGameState::~InGameState()
{
	RemoveEventListenType(Event_RestartLevel);
	RemoveEventListenType(Event_NextLevel);
	RemoveEventListenType(Event_App);
}

void InGameState::enter()
{
	GameState::enter();

	Game::GetInstance()->setMouseCursorVisible(false);

	_endState = false;
}

void InGameState::Render()
{
    GraphicsManager::getInstance()->Render();
    //PhysicsManager::GetInstance()->Render();
}

bool InGameState::Update(float deltaTime)
{
	InputManager::getInstance()->Update( deltaTime );
    PhysicsManager::GetInstance()->FixedUpdate( deltaTime );
	EventManager::GetInstance()->Update( deltaTime );
    EntityManager::getInstance()->Update( deltaTime );
	TaskManager::getInstance()->Update( deltaTime );

    return _endState;
}

void InGameState::Exit()
{
	GameState::Exit();

	Game::GetInstance()->setMouseCursorVisible(true);

	_endState = false;
}

bool InGameState::HandleEvent( const EventData& theevent )
{
	if((theevent.GetEventType() == Event_NextLevel || theevent.GetEventType() == Event_RestartLevel) && IsActive())
	{
		Game::GetInstance()->SetNextStateType(State_Loading);
		_endState = true;
	}

	if(theevent.GetEventType()==Event_App && IsActive())
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
