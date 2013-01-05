#include <Gwen/Controls/Button.h>
#include <Gwen/Controls/ImagePanel.h>
#include <Gwen/Controls/Label.h>
#include <Gwen/Controls/Rectangle.h>

#include "../GUI/MainMenu.hpp"
#include "../Base/Globals.hpp"
#include "../App/Game.hpp"
#include "../Event/NewGameEventData.hpp"

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
	newGameButton->SetText(L"새 게임");
	newGameButton->SetSize(250,100);
	newGameButton->SetPos(960 + 35,360);
	newGameButton->onPress.Add(this,&MainMenuControl::OnNewGame);

	Gwen::Controls::Button* continueButton = new Gwen::Controls::Button(this);
	continueButton->SetText(L"이어하기");
	continueButton->SetSize(250,100);
	continueButton->SetPos(960 + 35,470);
	continueButton->onPress.Add(this,&MainMenuControl::OnContinue);

	Gwen::Controls::Button* exitButton = new Gwen::Controls::Button(this);
	exitButton->SetText(L"끝내기");
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
	EventData* eventData = new NewGameEventData("Resource/Ogmo/Levels/Level1.oel");
	eventData->TriggerEvent();
}

void MainMenuControl::OnContinue( Gwen::Controls::Base* control )
{
}

