#include "../App/Game.hpp"

//Managers
#include "../System/GraphicsManager.hpp"
#include "../System/PhysicsManager.hpp"
#include "../System/GUIManager.hpp"

//Tile
#include "../Tile/Tile.hpp"

//States
#include "../State/IntroState.hpp"
#include "../State/MainMenuState.hpp"
#include "../State/InGameState.hpp"

#include <boost/lexical_cast.hpp>


SINGLETON_CONSTRUCTOR( Game ),
	sf::RenderWindow(),
	IEventListener("GameApp"),
	_isRunning( true ),
	_frameClock(),
	_gameClock(),
	_shouldSwitchState(false),
	_currentStateType(State_UNDEFINED),
	_nextStateType(State_UNDEFINED),
	_currentState(nullptr)
{
}

SINGLETON_DESTRUCTOR( Game )
{
}

void Game::Initialize( void )
{
	create(sf::VideoMode(SCREENWIDTH, SCREENHEIGHT),"Bounce",sf::Style::Close);

	GUIManager::GetInstance()->SetUpGUI();
    PhysicsManager::GetInstance()->SetUpPhysics();
    GraphicsManager::GetInstance()->SetUpGraphics();

    Tile::RegisterTileset("Rect", "Resource/Ogmo/Tiles/Rect.png");

	_stateMap.insert(std::make_pair(State_Intro,new IntroState));
	_stateMap.insert(std::make_pair(State_MainMenu,new MainMenuState));
	_stateMap.insert(std::make_pair(State_InGame,new InGameState));

	SetNextStateType(State_Intro);
	_shouldSwitchState = true;

	TRI_LOG_STR("Game initialized.");
}

void Game::Start( void )
{
	TRI_LOG_STR("Game started.");

    _frameClock.restart();
    _gameClock.restart();

    while(_isRunning)
    {
		ChangeStates();
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
            Quit();
        }

		_currentState->HandleAppEvent(windowEvent);
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

            assert(iter != _stateMap.end());

			if(_currentState)
			{
				_currentState->Exit();
			}
            _currentState = iter->second;
            _currentState->Enter();
        }
    }
}

void Game::Update(void)
{
    float deltaTime = _frameClock.restart().asSeconds();

    _shouldSwitchState = _currentState->Update(deltaTime);
}

void Game::Render( void )
{
    clear();

    _currentState->Render();

    display();
}
