#include <Gwen/Controls/Label.h>
#include <Gwen/Controls/Layout/Position.h>
#include <Gwen/Gwen.h>
#include "../Base/Globals.hpp"
#include "GUIManager.hpp"
#include "CreditsPage.hpp"

GWEN_CONTROL_CONSTRUCTOR(CreditsPageControl)
{
	SetBounds(0,0,SCREENWIDTH,SCREENHEIGHT);

	Gwen::Controls::Layout::Center* center = new Gwen::Controls::Layout::Center(this);
	center->SetBounds(0,0,SCREENWIDTH,SCREENHEIGHT);

	/*
	std::string creditsText =
		{	" \"moon.chase.star\" ", 
			"\nj.lee - IL JAE LEE, smashthewindow@gmail.com" };
	*/

	std::string creditsText =
		"	\"moon.chase.star\""
		"\n	"
		"\n	developers:"
		"\n	lead development / programming - Il Jae Lee (j.lee)"
		"\n	level design - Clayton Kramp (genki)"
		"\n	Kevin MacLeod, for releasing his works under Creative Commons license (incompetech.com)"
		"\n "
		"\n questions? comments? cries of anguish? smashthewindow@gmail.com";

	m_creditsFont = new Gwen::Font;
	m_creditsFont->facename = L"Resource/Fonts/Stroke-Bold.otf";
	m_creditsFont->size = 20;
	
	Gwen::Controls::Label* completeText = new Gwen::Controls::Label(center);
	completeText->SetTextColor(Gwen::Colors::White);
	completeText->SetText(creditsText);
	completeText->SetFont(m_creditsFont);
	completeText->SetAlignment(Gwen::Pos::CenterH);
	completeText->SizeToContents();
	completeText->SetPos(SCREENWIDTH/2 - completeText->GetSize().x/2, 300 );
}