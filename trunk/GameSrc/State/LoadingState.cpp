#include <Gwen/Controls/Rectangle.h>
#include <Gwen/Controls/Label.h>
#include <Gwen/Controls/Layout/Position.h>
#include "../State/LoadingState.hpp"
#include "../Base/Globals.hpp"
#include "../App/Game.hpp"
#include "../System/GUIManager.hpp"
#include "../System/SceneManager.hpp"
#include "../System/ResourceCache.hpp"
#include "../Event/GUIEventData.h"
#include "../Event/NewGameEventData.hpp"
#include "../System/EventManager.hpp"
#include "../System/GraphicsManager.hpp"

LoadingState::LoadingState() :	GameState(),
	_sceneNum(0),
	_loadType(Load_UNDEFINED),
	_screenBase(nullptr),
	_frameDrawn(false)
{
	addEventListenType(Event_NewGame);
	addEventListenType(Event_RestartLevel);
	addEventListenType(Event_NextLevel);
	addEventListenType(Event_GUI);

	_screenBase = new Gwen::Controls::Base(GUIManager::getInstance()->getCanvas());
	_screenBase->SetBounds(0,0,SCREENWIDTH,SCREENHEIGHT);

	Gwen::Controls::Rectangle* rectangle = new Gwen::Controls::Rectangle(_screenBase);
	rectangle->SetBounds(0,0,SCREENWIDTH,SCREENHEIGHT);
	rectangle->SetColor(Gwen::Colors::Black);

	Gwen::Controls::Layout::Center* center = new Gwen::Controls::Layout::Center(_screenBase);
	center->SetBounds(0,0,SCREENWIDTH,SCREENHEIGHT);

	Gwen::Controls::Label* label = new Gwen::Controls::Label(center);
	label->SetTextColor(Gwen::Colors::White);
	label->SetText(L"Loading...");
	label->SizeToContents();

	_screenBase->Hide();
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

	_frameDrawn = false;
	_screenBase->Show();
}

bool LoadingState::update( sf::Time deltaTime )
{
	if(_frameDrawn)
	{
		switch(_loadType)
		{
		case Load_New:
			{
				Game::GetInstance()->SetNextStateType(State_InGame);
				SceneManager::getInstance()->loadScene(_sceneNum);
				return true;
				break;
			}

		case Load_Restart:
			{
				Game::GetInstance()->SetNextStateType(State_InGame);
				SceneManager::getInstance()->restartScene();
				return true;
				break;
			}

		case Load_Unload:
			{
				EventData* unloadEvent = new EventData(Event_Unload);
				unloadEvent->triggerEvent();

				ResourceCache::DestroyInstance();

				Game::GetInstance()->SetNextStateType(State_MainMenu);
				return true;
				break;
			}

		case Load_Next:
			{
				EventData* unloadEvent = new EventData(Event_Unload);
				unloadEvent->triggerEvent();

				SceneManager::getInstance()->loadScene(_sceneNum);
				Game::GetInstance()->SetNextStateType(State_InGame);
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

	_frameDrawn = true;
}

void LoadingState::exit( void )
{
	GameState::exit();

	_frameDrawn = false;
	_screenBase->Hide();
}

bool LoadingState::handleEvent( const EventData& theevent )
{
	if(theevent.getEventType() == Event_NewGame )
	{
		const NewGameEventData& eventData = static_cast<const NewGameEventData&>(theevent);
		_sceneNum = eventData.GetSceneNumber();

		_loadType = Load_New;
	}

	if(theevent.getEventType() == Event_NextLevel)
	{
		_sceneNum = SceneManager::getInstance()->getLoadedSceneNumber() + 1;

		_loadType = Load_Next;
	}

	if(theevent.getEventType() == Event_RestartLevel)
	{
		_loadType = Load_Restart;
	}

	if(theevent.getEventType() == Event_GUI)
	{
		const GUIEventData& eventData = static_cast<const GUIEventData&>(theevent);
		std::string controlName = eventData.GetControl()->GetName();

		if( controlName == "MainMenuButton")
		{
			_loadType = Load_Unload;
		}
	}

	return false;
}

