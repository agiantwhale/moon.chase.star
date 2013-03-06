#include "License.hpp"
#include "../App/Game.hpp"
#include "../Base/Globals.hpp"
#include <TurboActivate.h>
#include <Gwen/Controls/Label.h>
#include <boost/algorithm/string.hpp>

// Support Unicode compilation and non-Windows compilation
#ifdef _WIN32
#include <tchar.h>
#else
#define _T(x) x
typedef char    TCHAR;
#endif

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
	Gwen::UnicodeString keyString = m_textBox->GetText().GetUnicode();
	boost::erase_all(keyString," ");
	boost::to_upper(keyString);
	HRESULT hr = CheckAndSavePKey(keyString.c_str(), TA_USER);
	if (hr == TA_OK)
	{
		// try to activate
		hr = Activate();

		if (hr == TA_OK)
		{
			DestroyModal();
			Hide();
		}
		else
		{
			m_textBox->SetText("activation error!");
		}
	}
	else
	{
		m_textBox->SetText("an unknown error!");
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