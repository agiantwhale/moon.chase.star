#include "GameState.hpp"
#include "../App/Game.hpp"

sb::GameState::GameState()
	:	m_active(false),
		m_enterTime(),
		m_exitTime()
{

}

sb::GameState::~GameState()
{

}

void sb::GameState::enter( void )
{
	m_active = true;
	m_enterTime = Game::getInstance()->getTime();
}

void sb::GameState::exit( void )
{
	m_active = false;
	m_exitTime = Game::getInstance()->getTime();
}
