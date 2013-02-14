#include <assert.h>
#include <sfTheora.h>
#include "../State/IntroState.hpp"
#include "../Base/Globals.hpp"
#include "../App/Game.hpp"
#include "../System/GUIManager.hpp"

IntroState::IntroState()
	:	_introVideo()
{
}

IntroState::~IntroState()
{
}

void IntroState::enter(void)
{
	GameState::enter();

    Game::GetInstance()->SetNextStateType(State_MainMenu);

	_introVideo.load("Resource/Videos/Intro.ogv");
	if( _introVideo.hasError() )
	{
		TRI_LOG_STR("Unable to load intro video file!");
	}

	_introVideo.play();
}

bool IntroState::update( sf::Time deltaTime )
{
	_introVideo.update(sf::seconds(deltaTime));

	return _introVideo.isDone();
}

void IntroState::render( void )
{
	Game::GetInstance()->draw(_introVideo);
}

void IntroState::exit(void)
{
	GameState::exit();
}