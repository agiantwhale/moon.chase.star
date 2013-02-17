#include <Gwen/Controls/Rectangle.h>
#include <Gwen/Controls/Label.h>
#include <Gwen/Controls/Layout/Position.h>
#include "../State/LoadingState.hpp"
#include "../Base/Globals.hpp"
#include "../App/Game.hpp"
#include "../GUI/GUIManager.hpp"
#include "../System/SceneManager.hpp"
#include "../System/ResourceCache.hpp"
#include "../Event/GUIEventData.h"
#include "../Event/NewGameEventData.hpp"
#include "../Event/EventManager.hpp"
#include "../System/GraphicsManager.hpp"

namespace sb
{
	LoadingState::LoadingState() :	GameState(),
		m_sceneNum(0),
		m_loadType(Load_UNDEFINED),
		m_screenBase(nullptr),
		m_frameDrawn(false)
	{
		addEventListenType(Event_NewGame);
		addEventListenType(Event_RestartLevel);
		addEventListenType(Event_NextLevel);
		addEventListenType(Event_GUI);

		m_screenBase = new Gwen::Controls::Base(GUIManager::getInstance()->getCanvas());
		m_screenBase->SetBounds(0,0,SCREENWIDTH,SCREENHEIGHT);

		Gwen::Controls::Rectangle* rectangle = new Gwen::Controls::Rectangle(m_screenBase);
		rectangle->SetBounds(0,0,SCREENWIDTH,SCREENHEIGHT);
		rectangle->SetColor(Gwen::Colors::Black);

		Gwen::Controls::Layout::Center* center = new Gwen::Controls::Layout::Center(m_screenBase);
		center->SetBounds(0,0,SCREENWIDTH,SCREENHEIGHT);

		Gwen::Controls::Label* label = new Gwen::Controls::Label(center);
		label->SetTextColor(Gwen::Colors::White);
		label->SetText(L"Loading...");
		label->SizeToContents();

		m_screenBase->Hide();
	}

	LoadingState::~LoadingState()
	{
		removeEventListenType(Event_NewGame);
		removeEventListenType(Event_RestartLevel);
		removeEventListenType(Event_NextLevel);
		removeEventListenType(Event_GUI);
	}

	void LoadingState::enter( void )
	{
		GameState::enter();

		m_frameDrawn = false;
		m_screenBase->Show();
	}

	bool LoadingState::update( sf::Time deltaTime )
	{
		if(m_frameDrawn)
		{
			switch(m_loadType)
			{
			case Load_New:
				{
					Game::getInstance()->setNextStateType(State_InGame);
					SceneManager::getInstance()->loadScene(m_sceneNum);
					return true;
					break;
				}

			case Load_Restart:
				{
					Game::getInstance()->setNextStateType(State_InGame);
					SceneManager::getInstance()->restartScene();
					return true;
					break;
				}

			case Load_Unload:
				{
					EventData* unloadEvent = new EventData(Event_Unload);
					unloadEvent->triggerEvent();

					ResourceCache::destroyInstance();

					Game::getInstance()->setNextStateType(State_MainMenu);
					return true;
					break;
				}

			case Load_Next:
				{
					EventData* unloadEvent = new EventData(Event_Unload);
					unloadEvent->triggerEvent();

					SceneManager::getInstance()->loadScene(m_sceneNum);
					Game::getInstance()->setNextStateType(State_InGame);
					return true;
					break;
				}

			default:
				{
					break;
				}
			}
		}

		return false;
	}

	void LoadingState::render( void )
	{
		GUIManager::getInstance()->render();

		m_frameDrawn = true;
	}

	void LoadingState::exit( void )
	{
		GameState::exit();

		m_frameDrawn = false;
		m_screenBase->Hide();
	}

	bool LoadingState::handleEvent( const EventData& theevent )
	{
		if(theevent.getEventType() == Event_NewGame )
		{
			const NewGameEventData& eventData = static_cast<const NewGameEventData&>(theevent);
			m_sceneNum = eventData.getSceneNumber();

			m_loadType = Load_New;
		}

		if(theevent.getEventType() == Event_NextLevel)
		{
			m_sceneNum = SceneManager::getInstance()->getLoadedSceneNumber() + 1;

			m_loadType = Load_Next;
		}

		if(theevent.getEventType() == Event_RestartLevel)
		{
			m_loadType = Load_Restart;
		}

		if(theevent.getEventType() == Event_GUI)
		{
			const GUIEventData& eventData = static_cast<const GUIEventData&>(theevent);
			std::string controlName = eventData.getControl()->GetName();

			if( controlName == "MainMenuButton")
			{
				m_loadType = Load_Unload;
			}
		}

		return false;
	}
}