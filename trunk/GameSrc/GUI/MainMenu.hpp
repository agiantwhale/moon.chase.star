#ifndef MainMenu_h__
#define MainMenu_h__

#include <Gwen/Controls/Base.h>

class MainMenuControl : public Gwen::Controls::Base
{
	GWEN_CONTROL(MainMenuControl,Gwen::Controls::Base);

public:
	void onContinue(Gwen::Controls::Base* control);
	void onNewGame(Gwen::Controls::Base* control);
	void onCredits(Gwen::Controls::Base* control);
	void onExit(Gwen::Controls::Base* control);
};

#endif // MainMenu_h__
