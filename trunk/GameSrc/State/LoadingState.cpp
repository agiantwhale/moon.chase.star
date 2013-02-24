#include <Gwen/Controls/Rectangle.h>
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
#include "../Physics/PhysicsManager.hpp"
#include "../System/InputManager.hpp"

namespace sb
{
	LoadingState::LoadingState() :	GameState(),
		m_sceneNum(0),
		m_loadType(Load_UNDEFINED),
		m_screenBase(nullptr),
		m_loadingText(nullptr),
		m_helperText(nullptr),
		m_pressEnterText(nullptr),
		m_frameDrawn(false),
		m_loadComplete(false)
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

		m_helperText = new Gwen::Controls::Label(center);
		m_helperText->SetTextColor(Gwen::Colors::White);
		m_helperText->SetText(L"Helper Text");
		m_helperText->SizeToContents();

		m_loadingText = new Gwen::Controls::Label(m_screenBase);
		m_loadingText->SetTextColor(Gwen::Colors::White);
		m_loadingText->SetText(L"loading...");
		m_loadingText->SetAlignment(Gwen::Pos::CenterH);
		m_loadingText->SizeToContents();
		m_loadingText->SetPos(SCREENWIDTH/2 - m_loadingText->GetSize().x/2, 400 );

		m_pressEnterText = new Gwen::Controls::Label(m_screenBase);
		m_pressEnterText->SetTextColor(Gwen::Colors::White);
		m_pressEnterText->SetText(L"press space to continue...");
		m_pressEnterText->SetAlignment(Gwen::Pos::CenterH);
		m_pressEnterText->SizeToContents();
		m_pressEnterText->SetPos(SCREENWIDTH/2 - m_pressEnterText->GetSize().x/2, 400 );

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
		m_loadComplete = false;
		m_screenBase->Show();

		m_loadingText->Show();
		m_pressEnterText->Hide();
		m_helperText->Hide();
	}

	bool LoadingState::update( sf::Time deltaTime )
	{
		if(m_frameDrawn)
		{
			InputManager::getInstance()->update(deltaTime);

			m_loadingText->Hide();
			m_pressEnterText->Show();

			if(!m_loadComplete)
			{
				m_loadComplete = true;
				
				switch(m_loadType)
				{
				case Load_New:
					{
						Game::getInstance()->setNextStateType(State_InGame);
						SceneManager::getInstance()->loadScene(m_sceneNum);

						break;
					}

				case Load_Restart:
					{
						Game::getInstance()->setNextStateType(State_InGame);
						SceneManager::getInstance()->restartScene();
						PhysicsManager::getInstance()->getPhysicsWorld()->SetGravity(b2Vec2(0,-GRAVITY_ACCELERATION));
						break;
					}

				case Load_Unload:
					{
						m_pressEnterText->Hide();

						EventData* unloadEvent = new EventData(Event_Unload);
						unloadEvent->triggerEvent();

						ResourceCache::destroyInstance();
						PhysicsManager::getInstance()->getPhysicsWorld()->SetGravity(b2Vec2(0,-GRAVITY_ACCELERATION));

						Game::getInstance()->setNextStateType(State_MainMenu);

						return true;
						break;
					}

				case Load_Next:
					{
						Game::getInstance()->setNextStateType(State_InGame);

						EventData* unloadEvent = new EventData(Event_Unload);
						unloadEvent->triggerEvent();

						ResourceCache::destroyInstance();
						PhysicsManager::getInstance()->getPhysicsWorld()->SetGravity(b2Vec2(0,-GRAVITY_ACCELERATION));

						if(m_sceneNum >= SceneManager::getInstance()->getMaximumSceneNum())
						{
							/*
							ResourceCache::destroyInstance();
							PhysicsManager::getInstance()->getPhysicsWorld()->SetGravity(b2Vec2(0,-GRAVITY_ACCELERATION));
							*/

							Game::getInstance()->setNextStateType(State_Credits);
							return true;
						}

						SceneManager::getInstance()->loadScene(m_sceneNum);
						break;
					}

				default:
					{
						break;
					}
				}
			}
			
			return InputManager::getInstance()->getContinueInput();
		}
		else
		{
			switch(m_loadType)
			{
			case Load_New:
				{
					m_helperText->Show();
					m_helperText->SetText(SceneManager::getInstance()->getHelperText(m_sceneNum));
					m_helperText->SizeToContents();

					break;
				}

			case Load_Restart:
				{
					m_helperText->Show();
					m_helperText->SetText(SceneManager::getInstance()->getHelperText(SceneManager::getInstance()->getLoadedSceneNumber()));
					m_helperText->SizeToContents();

					break;
				}

			case Load_Unload:
				{
					m_helperText->Hide();
					break;
				}

			case Load_Next:
				{
					m_helperText->Show();
					m_helperText->SetText(SceneManager::getInstance()->getHelperText(m_sceneNum));
					m_helperText->SizeToContents();
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