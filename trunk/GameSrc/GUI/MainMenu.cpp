#include <Gwen/Controls/Button.h>
#include "../GUI/MainMenu.hpp"
#include "../Base/Globals.hpp"

GWEN_CONTROL_CONSTRUCTOR(MainMenuWindowControl)
{
	m_Title->SetText("Main Menu");
	SetPos(SCREENWIDTH*3/4,0);
	SetSize(SCREENWIDTH/4,SCREENHEIGHT);

	Gwen::Controls::Button* newGameButton = new Gwen::Controls::Button(this);
	newGameButton->SetText("New Game");
	newGameButton->SetAlignment(Gwen::Pos::CenterH);
	newGameButton->SetSize(250,100);
	newGameButton->SetPos(0,360);
	newGameButton->onPress.Add(this,&MainMenuWindowControl::OnNewGame);
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

