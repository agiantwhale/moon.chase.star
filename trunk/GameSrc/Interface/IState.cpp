#include "../Interface/IState.hpp"
#include "../App/Game.hpp"

IState::IState() : _enterTime( 0.0f ), _exitTime( 0.0f )
{
}

IState::~IState()
{
}

void IState::Enter(void)
{
	_enterTime = Game::GetInstance()->GetTime();
}

void IState::Exit(void)
{
	_exitTime = Game::GetInstance()->GetTime();
}