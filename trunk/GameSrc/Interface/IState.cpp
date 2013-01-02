#include "../Interface/IState.hpp"
#include "../App/Game.hpp"

IState::IState() : _enterTime( 0.0f ), _exitTime( 0.0f ), _active(false)
{
}

IState::~IState()
{
}

void IState::Enter(void)
{
	_active = true;
    _enterTime = Game::GetInstance()->GetTime();
}

void IState::Exit(void)
{
	_active = false;
    _exitTime = Game::GetInstance()->GetTime();
}