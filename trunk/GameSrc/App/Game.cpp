#include "../App/Game.hpp"

//Managers
#include "../System/GraphicsManager.hpp"
#include "../System/PhysicsManager.hpp"
#include "../System/GUIManager.hpp"

#include "../Tile/Tile.hpp"


SINGLETON_CONSTRUCTOR( Game ),
	sf::RenderWindow(sf::VideoMode(SCREENWIDTH, SCREENHEIGHT),"Bounce",sf::Style::Close),
	_isRunning( true ),
	_frameClock(),
	_gameClock(),
	IEventListener("GameApp"),
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
	GUIManager::GetInstance()->SetUpGUI();
    PhysicsManager::GetInstance()->SetUpPhysics();
    GraphicsManager::GetInstance()->SetUpGraphics();

    Tile::RegisterTileset("Rect", "Resource/Ogmo/Tiles/Rect.png");

    LOG(INFO) << "Game initialized.";
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

		GUIManager::GetInstance()->FeedEvent(windowEvent);
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
}

void Game::Render( void )
{
    clear();

    //_currentState->Render();

	GUIManager::GetInstance()->Render();

    display();
}
