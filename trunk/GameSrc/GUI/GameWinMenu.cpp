#include <Gwen/Controls/Button.h>
#include <Gwen/Controls/ImagePanel.h>
#include <Gwen/Controls/Label.h>
#include <Gwen/Controls/Rectangle.h>
#include "GameWinMenu.hpp"
#include "../State/PauseState.hpp"
#include "../App/Game.hpp"
#include "../Base/Globals.hpp"
#include "../Event/GUIEventData.h"
#include "../Event/EventData.hpp"

GWEN_CONTROL_CONSTRUCTOR(GameWinMenuControl)
{
	SetBounds(0,0,SCREENWIDTH,SCREENHEIGHT);

	Gwen::Controls::Rectangle* darkRectangle = new Gwen::Controls::Rectangle(this);
	darkRectangle->SetBounds(0,0,960,720);
	darkRectangle->SetColor(Gwen::Color(0,0,0,80));

	Gwen::Controls::Rectangle* lightRectangle = new Gwen::Controls::Rectangle(this);
	lightRectangle->SetBounds(960,0,320,720);
	lightRectangle->SetColor(Gwen::Color(255,255,255,80));

	m_levelName = new Gwen::Controls::Label(lightRectangle);
	m_levelName->SetTextColor(Gwen::Colors::White);
	m_levelName->SetText(L"levelname");
	m_levelName->SetAlignment(Gwen::Pos::CenterH);
	m_levelName->SizeToContents();
	m_levelName->SetPos(320/2 - m_levelName->GetSize().x/2, 100 );

	Gwen::Controls::Label* completeText = new Gwen::Controls::Label(lightRectangle);
	completeText->SetTextColor(Gwen::Colors::White);
	completeText->SetText(L"complete!");
	completeText->SetAlignment(Gwen::Pos::CenterH);
	completeText->SizeToContents();
	completeText->SetPos(320/2 - completeText->GetSize().x/2, 150 );

	Gwen::Controls::Button* nextLevelButton = new Gwen::Controls::Button(this);
	nextLevelButton->SetText(L"next level");
	nextLevelButton->SetSize(250,100);
	nextLevelButton->SetPos(960 + 35,360);
	nextLevelButton->SetName("NextLevelButton");
	nextLevelButton->onPress.Add(this,&PauseMenuControl::onButtonPressed);

	Gwen::Controls::Button* restartButton = new Gwen::Controls::Button(this);
	restartButton->SetText(L"restart");
	restartButton->SetSize(250,100);
	restartButton->SetPos(960 + 35,470);
	restartButton->SetName("RestartButton");
	restartButton->onPress.Add(this,&PauseMenuControl::onButtonPressed);

	Gwen::Controls::Button* mainMenuButton = new Gwen::Controls::Button(this);
	mainMenuButton->SetText(L"to main menu");
	mainMenuButton->SetSize(250,100);
	mainMenuButton->SetPos(960 + 35,580);
	mainMenuButton->SetName("MainMenuButton");
	mainMenuButton->onPress.Add(this,&PauseMenuControl::onButtonPressed);
}

void GameWinMenuControl::onButtonPressed( Gwen::Controls::Base* control )
{
	sb::EventData* eventData = new sb::GUIEventData(control);
	eventData->triggerEvent();
}

void GameWinMenuControl::onExit( Gwen::Controls::Base* control )
{
	sb::Game::getInstance()->quit();
}

void GameWinMenuControl::setLevelName( const std::string& levelName )
{
	m_levelName->SetText(levelName);
	m_levelName->SetAlignment(Gwen::Pos::CenterH);
	m_levelName->SizeToContents();
	m_levelName->SetPos(320/2 - m_levelName->GetSize().x/2, 100 );
}

