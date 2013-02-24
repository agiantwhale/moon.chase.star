#include <assert.h>
#include <sfTheora.h>
#include "../State/IntroState.hpp"
#include "../Base/Globals.hpp"
#include "../App/Game.hpp"
#include "../GUI/GUIManager.hpp"

namespace sb
{
	IntroState::IntroState()
		:	m_introVideo()
	{
	}

	IntroState::~IntroState()
	{
	}

	void IntroState::enter(void)
	{
		GameState::enter();

		Game::getInstance()->setNextStateType(State_MainMenu);

		m_introVideo.load("Resource/Videos/Intro.ogv");
		if( m_introVideo.hasError() )
		{
			TRI_LOG_STR("Unable to load intro video file!");
		}

		m_introVideo.play();
	}

	bool IntroState::update( sf::Time deltaTime )
	{
		m_introVideo.update(deltaTime);

		return m_introVideo.isDone();
	}

	void IntroState::render( void )
	{
		Game::getInstance()->draw(m_introVideo);
	}

	void IntroState::exit(void)
	{
		GameState::exit();

		m_introVideo.unload();
	}
}