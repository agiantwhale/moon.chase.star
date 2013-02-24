#include <Gwen/Controls/Label.h>
#include <Gwen/Gwen.h>
#include "../Base/Globals.hpp"
#include "CreditsPage.hpp"

GWEN_CONTROL_CONSTRUCTOR(CreditsPageControl)
{
	SetBounds(0,0,SCREENWIDTH,SCREENHEIGHT);

	Gwen::Controls::Label* completeText = new Gwen::Controls::Label(this);
	completeText->SetTextColor(Gwen::Colors::White);
	completeText->SetText(L"sinabro presents");
	completeText->SetAlignment(Gwen::Pos::CenterH);
	completeText->SizeToContents();
	completeText->SetPos(SCREENWIDTH/2 - completeText->GetSize().x/2, 100 );

	completeText = new Gwen::Controls::Label(this);
	completeText->SetTextColor(Gwen::Colors::White);
	completeText->SetText(L"a game by j.lee");
	completeText->SetAlignment(Gwen::Pos::CenterH);
	completeText->SizeToContents();
	completeText->SetPos(SCREENWIDTH/2 - completeText->GetSize().x/2, 150 );

	completeText = new Gwen::Controls::Label(this);
	completeText->SetTextColor(Gwen::Colors::White);
	completeText->SetText(L"in collaboration with genki");
	completeText->SetAlignment(Gwen::Pos::CenterH);
	completeText->SizeToContents();
	completeText->SetPos(SCREENWIDTH/2 - completeText->GetSize().x/2, 200 );

	completeText = new Gwen::Controls::Label(this);
	completeText->SetTextColor(Gwen::Colors::White);
	completeText->SetText(L"\"moon.chase.star\"");
	completeText->SetAlignment(Gwen::Pos::CenterH);
	completeText->SizeToContents();
	completeText->SetPos(SCREENWIDTH/2 - completeText->GetSize().x/2, 300 );
}