#include <assert.h>

#include "../State/IntroState.hpp"
#include "../App/Game.hpp"
#include "../System/ResourceManager.hpp"

IntroState::IntroState() : _splashSprite( nullptr ), _splashTimer(), _currentStatus( FADE_IN )
{
}

IntroState::~IntroState()
{
    if( _splashSprite )
    {
        delete _splashSprite;
    }
}

void IntroState::Enter(void)
{
    Game::GetInstance()->SetNextStateType(State_MainMenu);

    _currentStatus = FADE_IN;
    _splashTimer.restart();
    _splashSprite = new sf::Sprite( *TextureManager::GetInstance()->GetResource( "Resource/Textures/Splash.png" ) );

    //TODO: Initialize sprite instance here.
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
        sf::Color spriteColor(255,255,255,255 * std::min( 1.0f, elapsedTime / FADE_TIME ) );
        _splashSprite->setColor( spriteColor );

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
        sf::Color spriteColor(255,255,255,255 * std::max( 0.0f, (FADE_TIME - elapsedTime) / FADE_TIME ) );
        _splashSprite->setColor( spriteColor );

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
    if( _splashSprite )
    {
        Game::GetInstance()->draw( *_splashSprite );
    }
}

void IntroState::Exit(void)
{
    delete _splashSprite;
    _splashSprite = nullptr;
}