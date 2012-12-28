#include "../App/Game.hpp"

//Managers
#include "../System/GraphicsManager.hpp"
#include "../System/EntityManager.hpp"
#include "../System/PhysicsManager.hpp"
#include "../System/ResourceManager.hpp"
#include "../System/SceneManager.hpp"
#include "../System/EventManager.hpp"

#include "../Tile/Tile.hpp"

#include <Gwen/Gwen.h>
#include <Gwen/Renderers/SFML.h>
#include <Gwen/Skins/TexturedBase.h>
#include <Gwen/Controls/Button.h>
#include <Gwen/Controls/WindowControl.h>


SINGLETON_CONSTRUCTOR( Game ), sf::RenderWindow(sf::VideoMode(SCREENWIDTH, SCREENHEIGHT),"Bounce",sf::Style::Close), _isRunning( true ), _frameClock(), _gameClock(),
                       IEventListener("GameApp"), _currentStateType(State_UNDEFINED), _nextStateType(State_UNDEFINED), _currentState(nullptr)
{
}

SINGLETON_DESTRUCTOR( Game )
{
}

void Game::Initialize( void )
{
    PhysicsManager::GetInstance()->SetUpPhysics();
    GraphicsManager::GetInstance()->SetUpGraphics();

    Tile::RegisterTileset("Rect", "Resource/Ogmo/Tiles/Rect.png");

    LOG(INFO) << "Game initialized.";

    EventManager* eventMgr = EventManager::GetInstance();
    eventMgr->AddListener( EntityManager::GetInstance(), Event_Unload );
    eventMgr->AddListener( GraphicsManager::GetInstance(), Event_Unload );
    eventMgr->AddListener( TextureManager::GetInstance(), Event_Unload );
    eventMgr->AddListener( SoundBufferManager::GetInstance(), Event_Unload );
    eventMgr->AddListener( SceneManager::GetInstance(), Event_Unload );

    eventMgr->AddListener( EntityManager::GetInstance(), Event_RestartLevel );
    eventMgr->AddListener( SceneManager::GetInstance(), Event_RestartLevel );
}

void Game::Start( void )
{
    _frameClock.restart();
    _gameClock.restart();

    while(_isRunning)
    {
        PollEvents();
        Update();
        Render();
    }
}

bool Game::HandleEvent(const EventData& theevent)
{
    return false;
}

void Game::PollEvents(void)
{
    sf::Event windowEvent;
    while(pollEvent(windowEvent))
    {
        if(windowEvent.type == sf::Event::Closed)
        {
            close();
            _isRunning = false;
        }

        sfml.ProcessMessage(windowEvent);
    }
}

void Game::ChangeStates(void)
{
    if(_shouldSwitchState)
    {
        _shouldSwitchState = false;

        if(_nextStateType != State_UNDEFINED)
        {
            _currentStateType = _nextStateType;
            _nextStateType = State_UNDEFINED;
            StateMap::const_iterator iter = _stateMap.find(_currentStateType);

            assert(iter == _stateMap.end());

            _currentState->Exit();
            _currentState = iter->second;
            _currentState->Enter();
        }
    }
}

void Game::Update(void)
{
    float deltaTime = _frameClock.restart().asSeconds();

    //_shouldSwitchState = _currentState->Update(deltaTime);

    /*
    PhysicsManager::GetInstance()->FixedUpdate( deltaTime );
    EntityManager::GetInstance()->Update( deltaTime );
    EventManager::GetInstance()->Update( deltaTime );
    */
}

void Game::Render( void )
{
    clear();

    pCanvas->RenderCanvas();

    //_currentState->Render();

    /*
    GraphicsManager::GetInstance()->Render();
    PhysicsManager::GetInstance()->Render();
    */

    display();
}
