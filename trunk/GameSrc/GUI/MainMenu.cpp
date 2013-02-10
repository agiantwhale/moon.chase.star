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

	Gwen::Controls::Rectangle* lightRectangle = new Gwen::Controls::Rectangle(this);
	lightRectangle->SetBounds(960,0,320,720);
	lightRectangle->SetColor(Gwen::Color(255,255,255,80));

	Gwen::Controls::ImagePanel* titleImage = new Gwen::Controls::ImagePanel(this);
	titleImage->SetImage("Resource/Textures/Title.png");
	titleImage->SetBounds(20,50,640,400);

	Gwen::Controls::Button* newGameButton = new Gwen::Controls::Button(this);
	newGameButton->SetText(L"New Game");
	newGameButton->SetSize(250,100);
	newGameButton->SetPos(960 + 35,250);
	newGameButton->onPress.Add(this,&MainMenuControl::OnNewGame);

	Gwen::Controls::Button* continueButton = new Gwen::Controls::Button(this);
	continueButton->SetText(L"Continue");
	continueButton->SetSize(250,100);
	continueButton->SetPos(960 + 35,360);
	continueButton->onPress.Add(this,&MainMenuControl::OnCredits);

	Gwen::Controls::Button* creditsButton = new Gwen::Controls::Button(this);
	creditsButton->SetText(L"Credits");
	creditsButton->SetSize(250,100);
	creditsButton->SetPos(960 + 35,470);
	creditsButton->SetName("Credits");
	creditsButton->onPress.Add(this,&MainMenuControl::OnContinue);

	Gwen::Controls::Button* exitButton = new Gwen::Controls::Button(this);
	exitButton->SetText(L"End Game");
	exitButton->SetSize(250,100);
	exitButton->SetPos(960 + 35,580);
	exitButton->onPress.Add(this,&MainMenuControl::OnExit);
}

/*
	Shouldn't the below functions be refactored?
*/

void MainMenuControl::OnExit( Gwen::Controls::Base* control )
{
	Game::GetInstance()->Quit();
}

void MainMenuControl::OnNewGame( Gwen::Controls::Base* control )
{
	EventData* eventData = new NewGameEventData(0);
	eventData->TriggerEvent();
}

void MainMenuControl::OnContinue( Gwen::Controls::Base* control )
{
	EventData* eventData = new NewGameEventData(SceneManager::GetInstance()->GetLoadedSceneNumber());
	eventData->TriggerEvent();
}

void MainMenuControl::OnCredits( Gwen::Controls::Base* control )
{
}
