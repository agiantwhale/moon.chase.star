#include "../App/Game.hpp"

#include <Windows.h>

#include <TinyXML/tinyxml.h>
#include <time.h>

//Event
#include "../Event/AppEventData.hpp"

//Managers
#include "../System/GraphicsManager.hpp"
#include "../Physics/PhysicsManager.hpp"
#include "../GUI/GUIManager.hpp"
#include "../System/SceneManager.hpp"
#include "../system/InputManager.hpp"
#include "../System/ActivationManager.hpp"

//Tile
#include "../Tile/Tile.hpp"

//States
#include "../State/IntroState.hpp"
#include "../State/MainMenuState.hpp"
#include "../State/InGameState.hpp"
#include "../State/GameWinState.hpp"
#include "../State/LoadingState.hpp"
#include "../State/PauseState.hpp"
#include "../State/CreditsState.hpp"

#include <boost/lexical_cast.hpp>
#include <shlobj.h>
#include <shlwapi.h>
#include <winerror.h>

namespace sb
{

	SINGLETON_CONSTRUCTOR( Game ),
		sf::RenderWindow(),
		m_isRunning( true ),
		m_isPaused( false ),
		m_frameClock(),
		m_gameClock(),
		m_shouldTakeScreenshot(false),
		m_shouldSwitchState(false),
		m_currentStateType(State_UNDEFINED),
		m_nextStateType(State_UNDEFINED),
		m_currentState(nullptr)
	{
	}

	SINGLETON_DESTRUCTOR( Game )
	{
	}

	void Game::initialize( void )
	{
		/*
		Settings gameSettings;
		gameSettings.fullscreen = false;
		*/

		bool loadSucess = false;
		bool fullScreen = false;
		bool vSync = false;

		TCHAR szPath[MAX_PATH];

		if(SUCCEEDED(
			
			SHGetFolderPath(NULL, 
			CSIDL_LOCAL_APPDATA, 
			NULL, 
			0, 
			szPath))) 
		{
			PathAppend(szPath, TEXT("mcs.xml"));

			char ch[MAX_PATH];
			char DefChar = ' ';
			WideCharToMultiByte(CP_ACP,0,szPath,-1, ch,260,&DefChar, NULL);

			loadSucess = m_appDocument.LoadFile(ch);
			if(!loadSucess)
			{
				/*
				if (CreateDirectory(szPath, NULL) ||
					ERROR_ALREADY_EXISTS == GetLastError())
				{
					CopyFile( L"mcs.xml", szPath, false );
					loadSucess = m_appDocument.LoadFile("mcs.xml");
				}
				*/

				CopyFile( L"mcs.xml", szPath, false );
				loadSucess = m_appDocument.LoadFile(ch);
			}

			/*
			TCHAR filePath[MAX_PATH];
			memset(filePath, 0x00, sizeof(filePath));
			GetModuleFileName(NULL, filePath, MAX_PATH);
			SetCurrentDirectory(filePath);
			*/

			if(loadSucess)
			{
				const TiXmlElement* settingsElement = m_appDocument.FirstChildElement("Settings");
				if(settingsElement)
				{
					settingsElement->QueryBoolAttribute("Fullscreen", &fullScreen);
					settingsElement->QueryBoolAttribute("VSync", &vSync);
				}
			}


		}

		unsigned int screenSettings = sf::Style::Close;
		if(fullScreen)
		{
			screenSettings = sf::Style::Close | sf::Style::Fullscreen;
		}
		create(sf::VideoMode(SCREENWIDTH, SCREENHEIGHT),"moon.chase.star",screenSettings);
		setVerticalSyncEnabled(vSync);

		if(loadSucess)
		{
			ActivationManager::getInstance()->setUpActivation(m_appDocument.FirstChildElement("Activation"));
			GUIManager::getInstance()->setUpGUI(m_appDocument.FirstChildElement("GUI"));
			PhysicsManager::getInstance()->setUpPhysics(m_appDocument.FirstChildElement("Physics"));
			GraphicsManager::getInstance()->setUpGraphics(m_appDocument.FirstChildElement("Graphics"));
			SceneManager::getInstance()->setUpScene(m_appDocument.FirstChildElement("Scene"));
			InputManager::getInstance()->setUpInput(m_appDocument.FirstChildElement("Input"));
		}

		Tile::registerTileset("Rect", "Resource/Ogmo/Tiles/Rect.png");
		Tile::registerTileset("Back", "Resource/Ogmo/Tiles/Back.png");

		m_stateMap.insert(std::make_pair(State_Intro,new IntroState));
		m_stateMap.insert(std::make_pair(State_MainMenu,new MainMenuState));
		m_stateMap.insert(std::make_pair(State_InGame,new InGameState));
		m_stateMap.insert(std::make_pair(State_GameWin,new GameWinState));
		m_stateMap.insert(std::make_pair(State_Loading,new LoadingState));
		m_stateMap.insert(std::make_pair(State_Paused,new PauseState));
		m_stateMap.insert(std::make_pair(State_Credits,new CreditsState));

		setNextStateType(State_Intro);
		m_shouldSwitchState = true;

		TRI_LOG_STR("Game initialized.");
	}

	void Game::start( void )
	{
		TRI_LOG_STR("Game started.");

		m_frameClock.restart();
		m_gameClock.restart();

		while(m_isRunning)
		{
			pollEvents();

			if(!m_isPaused)
			{
				changeStates();
				update();
			}

			render();
		}
	}

	void Game::pollEvents(void)
	{
		sf::Event windowEvent;
		while(pollEvent(windowEvent))
		{
			if(windowEvent.type == sf::Event::Closed)
			{
				quit();
			}

			if(windowEvent.type == sf::Event::LostFocus)
			{
				EventData* eventData = new EventData(Event_PauseApp);
				eventData->triggerEvent();
				m_isPaused = true;
			}

			if(windowEvent.type == sf::Event::GainedFocus)
			{
				EventData* eventData = new EventData(Event_ResumeApp);
				eventData->triggerEvent();
				m_isPaused = false;
			}

			if(windowEvent.type == sf::Event::KeyReleased)
			{
				if( windowEvent.key.code == sf::Keyboard::F10 )
				{
					m_shouldTakeScreenshot = true;
				}
			}

			EventData* eventData = new AppEventData(windowEvent);
			eventData->triggerEvent();
		}
	}

	void Game::changeStates(void)
	{
		if(m_shouldSwitchState)
		{
			m_shouldSwitchState = false;

			if(m_nextStateType != State_UNDEFINED)
			{
				m_currentStateType = m_nextStateType;
				m_nextStateType = State_UNDEFINED;
				StateMap::const_iterator iter = m_stateMap.find(m_currentStateType);

				assert(iter != m_stateMap.end());

				if(m_currentState)
				{
					m_currentState->exit();
				}
				m_currentState = iter->second;
				m_currentState->enter();
			}
		}
	}

	void Game::update(void)
	{
		sf::Time deltaTime = m_frameClock.getElapsedTime();
		m_frameClock.restart();

		m_shouldSwitchState = m_currentState->update(deltaTime);
	}

	void Game::render( void )
	{
		clear();

		m_currentState->render();

		if( m_shouldTakeScreenshot )
		{
			pauseFrameTimer();
			m_shouldTakeScreenshot = false;

			time_t rawtime;
			struct tm * timeinfo;
			char buffer [80];

			time ( &rawtime );
			timeinfo = localtime ( &rawtime );

			strftime (buffer,80,"screenshot_%Y_%d_%m_%H_%M_%S.png",timeinfo);

			sf::Image screenshot = capture();
			screenshot.saveToFile(buffer);
			resumeFrameTimer();
		}

		display();
	}

}