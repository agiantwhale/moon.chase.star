#include <assert.h>

#include "../State/IntroState.hpp"
#include "../Base/Globals.hpp"
#include "../App/Game.hpp"
#include "../System/ResourceManager.hpp"
#include "../System/GUIManager.hpp"

IntroState::IntroState() : _introMovie(nullptr)
{
}

IntroState::~IntroState()
{
	if(_introMovie)
	{
		delete _introMovie;
	}
}

void IntroState::Enter(void)
{
	IState::Enter();

    Game::GetInstance()->SetNextStateType(State_MainMenu);

    _introMovie = new sfe::Movie;
	if(_introMovie->openFromFile("Resource/Videos/Intro.wmv"))
	{
		TRI_LOG_STR("Unable to load intro video file!");
	}
	_introMovie->play();
}

bool IntroState::Update( float deltaTime )
{
    return _introMovie->getStatus() == sfe::Movie::Status::Stopped;
}

void IntroState::Render( void )
{
	Game::GetInstance()->draw(*_introMovie);
}

void IntroState::Exit(void)
{
	IState::Exit();
}