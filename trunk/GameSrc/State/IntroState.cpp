#include <assert.h>

#include "../State/IntroState.hpp"
#include "../Base/Globals.hpp"
#include "../App/Game.hpp"
#include "../System/ResourceManager.hpp"
#include "../System/GUIManager.hpp"

IntroState::IntroState() : _splashImage( nullptr ), _splashTimer(), _currentStatus( FADE_IN )
{
	_splashImage = new Gwen::Controls::ImagePanel(GUIManager::GetInstance()->GetCanvas());
	_splashImage->SetImage("Resource/Textures/Splash.png");
	_splashImage->SetBounds(0,0,SCREENWIDTH,SCREENHEIGHT);
	_splashImage->Hide();
}

IntroState::~IntroState()
{
	if( _splashImage )
		delete _splashImage;
}

void IntroState::Enter(void)
{
	IState::Enter();

    Game::GetInstance()->SetNextStateType(State_MainMenu);

    _currentStatus = FADE_IN;
    _splashTimer.restart();
	_splashImage->Show();
}

bool IntroState::Update( float deltaTime )
{
    const float elapsedTime = _splashTimer.getElapsedTime().asSeconds();
    const float FADE_TIME = 1.0f;
    const float MID_TIME = 1.0f;

    switch( _currentStatus )
    {
    case FADE_IN:
    {
        Gwen::Color imageColor(255,255,255,255 * std::min( 1.0f, elapsedTime / FADE_TIME ) );
        _splashImage->SetDrawColor(imageColor);

        if(elapsedTime > FADE_TIME)
        {
            _currentStatus = WAIT;
            _splashTimer.restart();
        }

        return false;
    }

    case WAIT:
    {
        if(elapsedTime > MID_TIME)
        {
            _currentStatus = FADE_OUT;
            _splashTimer.restart();
        }

        return false;
    }

    case FADE_OUT:
    {
		Gwen::Color imageColor(255,255,255,255 * std::max( 0.0f, FADE_TIME - elapsedTime / FADE_TIME ) );
		_splashImage->SetDrawColor(imageColor);

        if(elapsedTime > FADE_TIME)
        {
            return true;
        }

        return false;
    }

    default:
    {
        assert(false);
        break;
    }
    }

	return false;
}

void IntroState::Render( void )
{
	GUIManager::GetInstance()->Render();
}

void IntroState::Exit(void)
{
	IState::Exit();

	_splashImage->Hide();
}