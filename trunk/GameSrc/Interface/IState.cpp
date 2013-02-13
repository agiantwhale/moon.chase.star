#include "../Interface/IState.hpp"
#include "../App/Game.hpp"

GameState::GameState() : _enterTime( 0.0f ), _exitTime( 0.0f ), _active(false)
{
}

GameState::~GameState()
{
}

void GameState::enter(void)
{
	_active = true;
    _enterTime = Game::GetInstance()->GetTime();
}

void GameState::Exit(void)
{
	_active = false;
    _exitTime = Game::GetInstance()->GetTime();
}