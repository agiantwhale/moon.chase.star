#include <Gwen/Controls/Button.h>
#include <Gwen/Controls/ImagePanel.h>
#include <Gwen/Controls/Label.h>
#include <Gwen/Controls/Rectangle.h>
#include <Gwen/Controls/Layout/Position.h>

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

	m_levelSelectWindow = new LevelSelectMenuControl(this);
	m_levelSelectWindow->Hide();
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
	m_levelSelectWindow->updateListing();
	m_levelSelectWindow->Show();
	m_levelSelectWindow->MakeModal(true);
}

void MainMenuControl::onCredits( Gwen::Controls::Base* control )
{
	sb::EventData* eventData = new sb::EventData(Event_Credits);
	eventData->triggerEvent();
}

GWEN_CONTROL_CONSTRUCTOR(LevelSelectMenuControl)
{

	m_sceneNum = 0;

	const int WIDTH = 400;
	const int HEIGHT = 300;

	m_Title->SetText("");
	SetSize(WIDTH,HEIGHT);
	SetPos((SCREENWIDTH-WIDTH)/2,(SCREENHEIGHT-HEIGHT)/2);
	SetClosable(false);
	SetDeleteOnClose(false);
	DisableResizing();

	Gwen::Controls::Base* listBoxControl = new Gwen::Controls::Base(this);
	listBoxControl->Dock(Gwen::Pos::Fill);

	Gwen::Controls::Base* buttonControl = new Gwen::Controls::Base(this);
	buttonControl->Dock(Gwen::Pos::Bottom);
	buttonControl->SetHeight(75);

	Gwen::Controls::Layout::Position* positioner = new Gwen::Controls::Layout::Position(buttonControl);
	positioner->Dock(Gwen::Pos::Fill);

	m_levelListBox = new Gwen::Controls::ListBox(listBoxControl);
	m_levelListBox->SetMargin(Gwen::Margin(0,4,0,4));
	m_levelListBox->Dock(Gwen::Pos::Fill);
	m_levelListBox->SetAllowMultiSelect(false);
	updateListing();

	Gwen::Controls::Button* startButton = new Gwen::Controls::Button(positioner);
	startButton->SetText(L"start journey");
	startButton->Dock(Gwen::Pos::Left | Gwen::Pos::CenterV);
	startButton->SetMargin(Gwen::Margin(0,4,0,4));
	startButton->onPress.Add(this,&LevelSelectMenuControl::onStart);

	Gwen::Controls::Button* backButton = new Gwen::Controls::Button(positioner);
	backButton->SetText(L"back");
	backButton->Dock(Gwen::Pos::Right | Gwen::Pos::CenterV);
	backButton->SetMargin(Gwen::Margin(0,4,0,4));
	backButton->onPress.Add(this,&LevelSelectMenuControl::onBack);

	//crossSplitter->SetPanel(0,startButton);
	//crossSplitter->SetPanel(1,backButton);
	//startButton->Dock(Gwen::)
}

void LevelSelectMenuControl::onRowSelect(Gwen::Controls::Base* control)
{
	m_sceneNum = m_tableRowMap.find(control)->second;
}

void LevelSelectMenuControl::onStart( Gwen::Controls::Base* control )
{
	sb::EventData* eventData = new sb::NewGameEventData(m_sceneNum);
	eventData->triggerEvent();

	DestroyModal();
	Hide();
}

void LevelSelectMenuControl::onBack( Gwen::Controls::Base* control )
{
	DestroyModal();
	Hide();
}

void LevelSelectMenuControl::updateListing( void )
{
	m_levelListBox->Clear();
	m_tableRowMap.clear();

	unsigned int sceneCount = 0;
	for(std::vector<sb::SceneInfo>::const_iterator iter = sb::SceneManager::getInstance()->getSceneInfoStack().begin(); iter != sb::SceneManager::getInstance()->getSceneInfoStack().end(); iter++ )
	{
		if(sceneCount <= sb::SceneManager::getInstance()->getClearedSceneNum())
		{
			const sb::SceneInfo& info = (*iter);
			Gwen::Controls::Layout::TableRow* row = m_levelListBox->AddItem(info.sceneName, info.sceneName);
			row->onRowSelected.Add(this, &LevelSelectMenuControl::onRowSelect);
			//row->UserData.Set<unsigned int>("sceneNum", sceneCount);

			m_tableRowMap.insert(std::make_pair(row, sceneCount));

			sceneCount++;
		}
		else
		{
			break;
		}
	}
}
