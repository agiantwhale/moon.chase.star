#include <assert.h>

#include "../State/IntroState.hpp"
#include "../Base/Globals.hpp"
#include "../App/Game.hpp"
#include "../System/GUIManager.hpp"

IntroState::IntroState() : _introMovie()
{
}

IntroState::~IntroState()
{
}

void IntroState::Enter(void)
{
	IState::Enter();

    Game::GetInstance()->SetNextStateType(State_MainMenu);

	if(!_introMovie.openFromFile("Resource/Videos/Intro.wmv"))
	{
		TRI_LOG_STR("Unable to load intro video file!");
	}
	_introMovie.resizeToFrame(0, 0, SCREENWIDTH, SCREENHEIGHT);
	_introMovie.play();
}

bool IntroState::Update( float deltaTime )
{
    return _introMovie.getStatus() == sfe::Movie::Status::Stopped;
}

void IntroState::Render( void )
{
	Game::GetInstance()->draw(_introMovie);
}

void IntroState::Exit(void)
{
	IState::Exit();
}