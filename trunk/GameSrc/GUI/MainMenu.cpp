#include <Gwen/Controls/Button.h>
#include <Gwen/Controls/ImagePanel.h>
#include <Gwen/Controls/Label.h>
#include <Gwen/Controls/Rectangle.h>
#include "../GUI/MainMenu.hpp"
#include "../State/MainMenuState.hpp"
#include "../Base/Globals.hpp"
#include "../App/Game.hpp"
#include "../System/SceneManager.hpp"

GWEN_CONTROL_CONSTRUCTOR(MainMenuControl)
{
	_ownerState = nullptr;

	SetBounds(0,0,SCREENWIDTH,SCREENHEIGHT);

	Gwen::Controls::ImagePanel* titleImage = new Gwen::Controls::ImagePanel(this);
	titleImage->SetImage("Resource/Textures/Title.png");
	titleImage->SetBounds(20,50,640,400);

	Gwen::Controls::Rectangle* lightRectangle = new Gwen::Controls::Rectangle(this);
	lightRectangle->SetBounds(960,0,320,720);
	lightRectangle->SetColor(Gwen::Color(255,255,255,60));

	Gwen::Controls::Button* newGameButton = new Gwen::Controls::Button(this);
	newGameButton->SetText("New Game");
	newGameButton->SetSize(250,100);
	newGameButton->SetPos(960 + 35,360);
	newGameButton->onPress.Add(this,&MainMenuControl::OnNewGame);

	Gwen::Controls::Button* continueButton = new Gwen::Controls::Button(this);
	continueButton->SetText("Continue");
	continueButton->SetSize(250,100);
	continueButton->SetPos(960 + 35,470);
	continueButton->onPress.Add(this,&MainMenuControl::OnContinue);

	Gwen::Controls::Button* exitButton = new Gwen::Controls::Button(this);
	exitButton->SetText("Exit");
	exitButton->SetSize(250,100);
	exitButton->SetPos(960 + 35,580);
	exitButton->onPress.Add(this,&MainMenuControl::OnExit);
}

void MainMenuControl::OnExit( Gwen::Controls::Base* control )
{
	Game::GetInstance()->Quit();
}

void MainMenuControl::OnNewGame( Gwen::Controls::Base* control )
{
	Game::GetInstance()->SetNextStateType(State_InGame);
	SceneManager::GetInstance()->LoadScene("Resource/Ogmo/Levels/Level3.oel");
	_ownerState->EndState();
}

void MainMenuControl::OnContinue( Gwen::Controls::Base* control )
{
}

