#include <Gwen/Controls/Button.h>
#include <Gwen/Controls/ImagePanel.h>
#include <Gwen/Controls/Label.h>
#include <Gwen/Controls/Rectangle.h>
#include "../State/PauseState.hpp"
#include "../App/Game.hpp"
#include "../GUI/PauseMenu.hpp"
#include "../Base/Globals.hpp"
#include "../Event/GUIEventData.h"
#include "../Event/EventData.hpp"

GWEN_CONTROL_CONSTRUCTOR(PauseMenuControl)
{
	SetBounds(0,0,SCREENWIDTH,SCREENHEIGHT);

	Gwen::Controls::Rectangle* darkRectangle = new Gwen::Controls::Rectangle(this);
	darkRectangle->SetBounds(0,0,960,720);
	darkRectangle->SetColor(Gwen::Color(0,0,0,80));

	Gwen::Controls::Rectangle* lightRectangle = new Gwen::Controls::Rectangle(this);
	lightRectangle->SetBounds(960,0,320,720);
	lightRectangle->SetColor(Gwen::Color(255,255,255,80));

	Gwen::Controls::Button* resumeButton = new Gwen::Controls::Button(this);
	resumeButton->SetText(L"resume");
	resumeButton->SetSize(250,100);
	resumeButton->SetPos(960 + 35,250);
	resumeButton->SetName("ResumeButton");
	resumeButton->onPress.Add(this,&PauseMenuControl::onButtonPressed);

	Gwen::Controls::Button* restartButton = new Gwen::Controls::Button(this);
	restartButton->SetText(L"restart");
	restartButton->SetSize(250,100);
	restartButton->SetPos(960 + 35,360);
	restartButton->SetName("RestartButton");
	restartButton->onPress.Add(this,&PauseMenuControl::onButtonPressed);

	Gwen::Controls::Button* mainMenuButton = new Gwen::Controls::Button(this);
	mainMenuButton->SetText(L"to main menu");
	mainMenuButton->SetSize(250,100);
	mainMenuButton->SetPos(960 + 35,470);
	mainMenuButton->SetName("MainMenuButton");
	mainMenuButton->onPress.Add(this,&PauseMenuControl::onButtonPressed);

	Gwen::Controls::Button* exitButton = new Gwen::Controls::Button(this);
	exitButton->SetText(L"end game");
	exitButton->SetSize(250,100);
	exitButton->SetPos(960 + 35,580);
	exitButton->SetName("ExitButton");
	exitButton->onPress.Add(this,&PauseMenuControl::onExit);
}

void PauseMenuControl::onButtonPressed( Gwen::Controls::Base* control )
{
	sb::EventData* eventData = new sb::GUIEventData(control);
	eventData->triggerEvent();
}

void PauseMenuControl::onExit( Gwen::Controls::Base* control )
{
	sb::Game::getInstance()->quit();
}

