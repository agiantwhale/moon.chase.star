#include <Gwen/Controls/Button.h>
#include <Gwen/Controls/ImagePanel.h>
#include <Gwen/Controls/Label.h>
#include <Gwen/Controls/Rectangle.h>

#include "../GUI/MainMenu.hpp"
#include "../Base/Globals.hpp"
#include "../App/Game.hpp"
#include "../Event/NewGameEventData.hpp"
#include "../Event/GUIEventData.h"
#include "../System/SceneManager.hpp"

GWEN_CONTROL_CONSTRUCTOR(MainMenuControl)
{
	SetBounds(0,0,SCREENWIDTH,SCREENHEIGHT);

	/*
	Gwen::Controls::Rectangle* lightRectangle = new Gwen::Controls::Rectangle(this);
	lightRectangle->SetBounds(960,0,320,720);
	lightRectangle->SetColor(Gwen::Color(255,255,255,80));
	*/

	Gwen::Controls::ImagePanel* titleImage = new Gwen::Controls::ImagePanel(this);
	titleImage->SetImage("Resource/Textures/Logo.png");
	titleImage->SetBounds(SCREENWIDTH/2 - 150,50,300,300);

	Gwen::Controls::Button* newGameButton = new Gwen::Controls::Button(this);
	newGameButton->SetText(L"new game");
	newGameButton->SetSize(250,50);
	newGameButton->SetPos(SCREENWIDTH/2 - 125,400);
	newGameButton->onPress.Add(this,&MainMenuControl::onNewGame);

	Gwen::Controls::Button* continueButton = new Gwen::Controls::Button(this);
	continueButton->SetText(L"continue");
	continueButton->SetSize(250,50);
	continueButton->SetPos(SCREENWIDTH/2 - 125,475);
	continueButton->onPress.Add(this,&MainMenuControl::onContinue);

	Gwen::Controls::Button* creditsButton = new Gwen::Controls::Button(this);
	creditsButton->SetText(L"credits");
	creditsButton->SetSize(250,50);
	creditsButton->SetPos(SCREENWIDTH/2 - 125,550);
	creditsButton->SetName("Credits");
	creditsButton->onPress.Add(this,&MainMenuControl::onCredits);

	Gwen::Controls::Button* exitButton = new Gwen::Controls::Button(this);
	exitButton->SetText(L"end game");
	exitButton->SetSize(250,50);
	exitButton->SetPos(SCREENWIDTH/2 - 125,625);
	exitButton->onPress.Add(this,&MainMenuControl::onExit);
}

/*
	Shouldn't the below functions be refactored?
*/

void MainMenuControl::onExit( Gwen::Controls::Base* control )
{
	sb::Game::getInstance()->quit();
}

void MainMenuControl::onNewGame( Gwen::Controls::Base* control )
{
	sb::EventData* eventData = new sb::NewGameEventData(0);
	eventData->triggerEvent();
}

void MainMenuControl::onContinue( Gwen::Controls::Base* control )
{
	sb::EventData* eventData = new sb::NewGameEventData(sb::SceneManager::getInstance()->getLoadedSceneNumber());
	eventData->triggerEvent();
}

void MainMenuControl::onCredits( Gwen::Controls::Base* control )
{
	sb::EventData* eventData = new sb::EventData(Event_Credits);
	eventData->triggerEvent();
}
