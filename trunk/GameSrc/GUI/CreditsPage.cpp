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
		"\n	"
		"\n	"
		"\n	developers:"
		"\n	lead development / programming - Il Jae Lee (j.lee)"
		"\n	level design - Clayton Kramp (genki)"
		"\n	"
		"\n	"
		"\n	"
		"\n	special thanks to:"
		"\n	whoever wrote that poem about chasing stars on Birmingham Starbucks"
		"\n	Joe St. Germain, for the beta testing and breaking builds of the game"
		"\n	Laurent Gomila, for making this awesome rendering library SFML"
		"\n	Erin Catto, the genius who wrote Box2D"
		"\n	Kevin MacLeod, for releasing his works under Creative Commons license (incompetech.com)"
		"\n	Joohee Jin, for listening to my complaints during the development"
		"\n	... and YOU, for being part of this creative communication"
		"\n	"
		"\n	"
		"\n	"
		"\n	remembering Caleb, forever shining in our hearts"
		"\n	"
		"\n	"
		"\n	"
		"\n	thanks all for playing, it's been a fun ride."
		"\n	- 2013.02.25, j & genki from Cranbrook";

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