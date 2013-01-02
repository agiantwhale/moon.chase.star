#include <Gwen/Controls/Rectangle.h>
#include <Gwen/Controls/Label.h>
#include <Gwen/Controls/Layout/Position.h>
#include "../State/LoadingState.hpp"
#include "../Base/Globals.hpp"
#include "../App/Game.hpp"
#include "../Event/NewGameEventData.hpp"
#include "../System/GUIManager.hpp"
#include "../System/SceneManager.hpp"
#include "../System/ResourceManager.hpp"
#include "../Event/GUIEventData.h"

LoadingState::LoadingState() :	IState(),
								_sceneFileName(""),
								_loadType(Load_UNDEFINED),
								_screenBase(nullptr),
								_frameDrawn(false)
{
	AddEventListenType(Event_NewGame);
	AddEventListenType(Event_RestartLevel);
	AddEventListenType(Event_GUI);

	_screenBase = new Gwen::Controls::Base(GUIManager::GetInstance()->GetCanvas());
	_screenBase->SetBounds(0,0,SCREENWIDTH,SCREENHEIGHT);

	Gwen::Controls::Rectangle* rectangle = new Gwen::Controls::Rectangle(_screenBase);
	rectangle->SetBounds(0,0,SCREENWIDTH,SCREENHEIGHT);
	rectangle->SetColor(Gwen::Colors::Black);

	Gwen::Controls::Layout::Center* center = new Gwen::Controls::Layout::Center(_screenBase);
	center->SetBounds(0,0,SCREENWIDTH,SCREENHEIGHT);

	Gwen::Controls::Label* label = new Gwen::Controls::Label(center);
	label->SetTextColor(Gwen::Colors::White);
	label->SetText(L"불러오는 중...");
	label->SizeToContents();

	_screenBase->Hide();
}

LoadingState::~LoadingState()
{

}

void LoadingState::Enter( void )
{
	IState::Enter();

	_frameDrawn = false;
	_screenBase->Show();
}

bool LoadingState::Update( float deltaTime )
{
	if(_frameDrawn)
	{
		switch(_loadType)
		{
		case Load_New:
			{
				Game::GetInstance()->SetNextStateType(State_InGame);
				SceneManager::GetInstance()->LoadScene(_sceneFileName);
				return true;
				break;
			}

		case Load_Restart:
			{
				Game::GetInstance()->SetNextStateType(State_InGame);
				SceneManager::GetInstance()->RestartScene();
				return true;
				break;
			}

		case Load_Unload:
			{
				EventData* unloadEvent = new EventData(Event_Unload);
				unloadEvent->TriggerEvent();

				/*
				Add code to load mainmenu later.
				*/

				Game::GetInstance()->SetNextStateType(State_MainMenu);
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

void LoadingState::Render( void )
{
	GUIManager::GetInstance()->Render();

	_frameDrawn = true;
}

void LoadingState::Exit( void )
{
	IState::Exit();

	_frameDrawn = false;
	_screenBase->Hide();
}

bool LoadingState::HandleEvent( const EventData& theevent )
{
	if(theevent.GetEventType() == Event_NewGame || theevent.GetEventType() == Event_NextLevel)
	{
		const NewGameEventData& eventData = static_cast<const NewGameEventData&>(theevent);
		_sceneFileName = eventData.GetSceneFileName();

		_loadType = Load_New;
	}

	if(theevent.GetEventType() == Event_RestartLevel)
	{
		_loadType = Load_Restart;
	}

	if(theevent.GetEventType() == Event_GUI)
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

