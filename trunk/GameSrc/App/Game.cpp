#include "../App/Game.hpp"

#include <TinyXML/tinyxml.h>

//Event
#include "../Event/AppEventData.hpp"

//Managers
#include "../System/GraphicsManager.hpp"
#include "../System/PhysicsManager.hpp"
#include "../System/GUIManager.hpp"
#include "../System/SceneManager.hpp"

//Tile
#include "../Tile/Tile.hpp"

//States
#include "../State/IntroState.hpp"
#include "../State/MainMenuState.hpp"
#include "../State/InGameState.hpp"
#include "../State/LoadingState.hpp"
#include "../State/PauseState.hpp"

#include <boost/lexical_cast.hpp>


SINGLETON_CONSTRUCTOR( Game ),
	sf::RenderWindow(),
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
	Settings gameSettings;
	gameSettings.fullscreen = false;
	TiXmlDocument document;
	if(document.LoadFile("MCS.xml"))
	{
		const TiXmlElement* settingsElement = document.FirstChildElement("Settings");
		if(settingsElement)
		{
			settingsElement->QueryBoolAttribute("Fullscreen", &gameSettings.fullscreen);
			settingsElement->QueryIntAttribute("MaxFramerate", &gameSettings.maxFramerate);
		}
	}

	unsigned int screenSettings = sf::Style::Close;
	if(gameSettings.fullscreen)
	{
		screenSettings = sf::Style::Close | sf::Style::Fullscreen;
	}
	create(sf::VideoMode(SCREENWIDTH, SCREENHEIGHT),"moon.chase.star",screenSettings);

	if(gameSettings.maxFramerate != 0)
	{
		setFramerateLimit(gameSettings.maxFramerate);
	}

	GUIManager::GetInstance()->SetUpGUI();
    PhysicsManager::GetInstance()->SetUpPhysics();
    GraphicsManager::GetInstance()->SetUpGraphics();
	SceneManager::GetInstance()->SetUpScene();

    Tile::RegisterTileset("Rect", "Resource/Ogmo/Tiles/Rect.png");
	Tile::RegisterTileset("Back", "Resource/Ogmo/Tiles/Back.png");

	_stateMap.insert(std::make_pair(State_Intro,new IntroState));
	_stateMap.insert(std::make_pair(State_MainMenu,new MainMenuState));
	_stateMap.insert(std::make_pair(State_InGame,new InGameState));
	_stateMap.insert(std::make_pair(State_Loading,new LoadingState));
	_stateMap.insert(std::make_pair(State_Paused,new PauseState));

	SetNextStateType(State_Intro);
	_shouldSwitchState = true;

	setFramerateLimit(30);

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

void Game::PollEvents(void)
{
    sf::Event windowEvent;
    while(pollEvent(windowEvent))
    {
        if(windowEvent.type == sf::Event::Closed)
        {
            Quit();
        }

		EventData* eventData = new AppEventData(windowEvent);
		eventData->TriggerEvent();
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
    float deltaTime = _frameClock.getElapsedTime().asSeconds();
	_frameClock.restart();

    _shouldSwitchState = _currentState->Update(deltaTime);
}

void Game::Render( void )
{
    clear();

    _currentState->Render();

	/*
	static int screen = 0;
	if( sf::Keyboard::isKeyPressed(sf::Keyboard::F10) )
	{
		screen++;
		capture().saveToFile("Screenie" + boost::lexical_cast<string>(screen) + ".tga");
	}
	*/

    display();
}
