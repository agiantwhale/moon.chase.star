#include "../State/InGameState.hpp"
#include "../App/Game.hpp"
#include "../System/GraphicsManager.hpp"
#include "../System/EntityManager.hpp"
#include "../System/PhysicsManager.hpp"
#include "../System/ResourceManager.hpp"
#include "../System/SceneManager.hpp"
#include "../System/EventManager.hpp"
#include "../System/TaskManager.hpp"
#include "../Event/AppEventData.hpp"

InGameState::InGameState() :	IState(),
								IEventListener("InGameState"),
								_endState(false)
{
	AddEventListenType(Event_RestartLevel);
	AddEventListenType(Event_App);
}

InGameState::~InGameState()
{}

void InGameState::Enter()
{
	IState::Enter();

	_endState = false;
}

void InGameState::Render()
{
    GraphicsManager::GetInstance()->Render();

#ifdef _DEBUG
    //PhysicsManager::GetInstance()->Render();
#endif
}

bool InGameState::Update(float deltaTime)
{
    PhysicsManager::GetInstance()->FixedUpdate( deltaTime );
    EntityManager::GetInstance()->Update( deltaTime );
    EventManager::GetInstance()->Update( deltaTime );
	TaskManager::GetInstance()->Update( deltaTime );

    return _endState;
}

void InGameState::Exit()
{
	IState::Exit();

	_endState = false;
}

bool InGameState::HandleEvent( const EventData& theevent )
{
	if(theevent.GetEventType() == Event_RestartLevel)
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
