#include "License.hpp"
#include "../App/Game.hpp"
#include "../Base/Globals.hpp"
#include "../System/ActivationManager.hpp"
#include "boost/algorithm/string/erase.hpp"
#include "boost/algorithm/string/case_conv.hpp"

GWEN_CONTROL_CONSTRUCTOR(ActivateControl)
{
	const int WIDTH = 400;
	const int HEIGHT = 100;

	m_Title->SetText("");
	SetSize(WIDTH,HEIGHT);
	SetPos((SCREENWIDTH-WIDTH)/2,(SCREENHEIGHT-HEIGHT)/2);
	SetClosable(false);
	SetDeleteOnClose(false);
	DisableResizing();

	Gwen::Controls::TextBox* activateKey = new Gwen::Controls::TextBox(this);
	activateKey->Dock(Gwen::Pos::Fill);
	activateKey->SetText("input activation key here...");
	m_textBox = activateKey;

	Gwen::Controls::Button* activateButton = new Gwen::Controls::Button(this);
	activateButton->SetText(L"activate");
	activateButton->Dock(Gwen::Pos::Bottom);
	activateButton->SetMargin(Gwen::Margin(0,4,0,4));
	activateButton->onPress.Add(this,&ActivateControl::onSend);
}

void ActivateControl::onSend( Gwen::Controls::Base* control )
{
	std::string key = m_textBox->GetText().Get();
	boost::erase_all(key," ");
	boost::to_upper(key);

	sb::ActivationManager::getInstance()->activate(key);

	if( sb::ActivationManager::getInstance()->isGenuine() )
	{
		sb::ActivationManager::getInstance()->writeToFile(key);
		DestroyModal();
		Hide();
	}
	else
	{
		m_textBox->SetText("activation error!");
	}
}

GWEN_CONTROL_CONSTRUCTOR(ConnectToInternetControl)
{
	const int WIDTH = 400;
	const int HEIGHT = 100;

	m_Title->SetText("");
	SetSize(WIDTH,HEIGHT);
	SetPos((SCREENWIDTH-WIDTH)/2,(SCREENHEIGHT-HEIGHT)/2);
	SetClosable(false);
	SetDeleteOnClose(false);
	DisableResizing();

	Gwen::Controls::Label* connectText = new Gwen::Controls::Label(this);
	connectText->Dock(Gwen::Pos::Fill);
	connectText->SetText("please connect to the internet and restart the game.");

	Gwen::Controls::Button* endButton = new Gwen::Controls::Button(this);
	endButton->SetText(L"exit game");
	endButton->Dock(Gwen::Pos::Bottom);
	endButton->SetMargin(Gwen::Margin(0,4,0,4));
	endButton->onPress.Add(this,&ConnectToInternetControl::onExit);
}

void ConnectToInternetControl::onExit( Gwen::Controls::Base* control )
{
	sb::Game::getInstance()->quit();
}