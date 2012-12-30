#include <Gwen/Controls/Button.h>
#include <Gwen/Controls/Layout/Position.h>
#include "../GUI/MainMenu.hpp"
#include "../Base/Globals.hpp"

GWEN_CONTROL_CONSTRUCTOR(MainMenuWindowControl)
{
	m_Title->SetText("Main Menu");
	SetPos(SCREENWIDTH*3/4,0);
	SetSize(SCREENWIDTH/4,SCREENHEIGHT);

	//Gwen::Controls::Layout::Center* centerLayout = new Gwen::Controls::Layout::Center(this);
	//centerLayout->Dock(Gwen::Pos::Fill);

	Gwen::Controls::Button* newGameButton = new Gwen::Controls::Button(this);
	newGameButton->SetText("New Game");
	newGameButton->SetSize(250,100);
	newGameButton->SetPos(35,360);
	newGameButton->onPress.Add(this,&MainMenuWindowControl::OnNewGame);

	Gwen::Controls::Button* continueButton = new Gwen::Controls::Button(this);
	continueButton->SetText("Continue");
	continueButton->SetSize(250,100);
	continueButton->SetPos(35,470);
	continueButton->onPress.Add(this,&MainMenuWindowControl::OnContinue);

	Gwen::Controls::Button* exitButton = new Gwen::Controls::Button(this);
	exitButton->SetText("Exit");
	exitButton->SetSize(250,100);
	exitButton->SetPos(35,580);
	exitButton->onPress.Add(this,&MainMenuWindowControl::OnExit);
}

void MainMenuWindowControl::OnExit( Gwen::Controls::Base* control )
{

}

void MainMenuWindowControl::OnNewGame( Gwen::Controls::Base* control )
{

}

void MainMenuWindowControl::OnContinue( Gwen::Controls::Base* control )
{

}

