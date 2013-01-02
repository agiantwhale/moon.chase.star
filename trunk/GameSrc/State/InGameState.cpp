#include "../State/InGameState.hpp"
#include "../App/Game.hpp"
#include "../System/GraphicsManager.hpp"
#include "../System/EntityManager.hpp"
#include "../System/PhysicsManager.hpp"
#include "../System/ResourceManager.hpp"
#include "../System/SceneManager.hpp"
#include "../System/EventManager.hpp"

InGameState::InGameState() :	IState(),
								IEventListener("InGameState"),
								_endState(false)
{
	AddEventListenType(Event_RestartLevel);
}

InGameState::~InGameState()
{}

void InGameState::Enter()
{
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
	if( sf::Keyboard::isKeyPressed(sf::Keyboard::Escape) )
	{
		Game::GetInstance()->SetNextStateType(State_Paused);
		return true;
	}

    PhysicsManager::GetInstance()->FixedUpdate( deltaTime );
    EntityManager::GetInstance()->Update( deltaTime );
    EventManager::GetInstance()->Update( deltaTime );

    return _endState;
}

void InGameState::Exit()
{
	_endState = false;
}

bool InGameState::HandleEvent( const EventData& theevent )
{
	if(theevent.GetEventType() == Event_RestartLevel)
	{
		Game::GetInstance()->SetNextStateType(State_Loading);
		_endState = true;
	}

	return false;
}
