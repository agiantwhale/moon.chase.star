#include "../State/InGameState.hpp"
#include "../System/GraphicsManager.hpp"
#include "../System/EntityManager.hpp"
#include "../System/PhysicsManager.hpp"
#include "../System/ResourceManager.hpp"
#include "../System/SceneManager.hpp"
#include "../System/EventManager.hpp"

InGameState::InGameState() : IState()
{

}

InGameState::~InGameState()
{}

void InGameState::Enter()
{

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

    return false;
}

void InGameState::Exit()
{
}