#ifndef MAINMENU_HPP
#define MAINMENU_HPP

#include <Gwen/Controls/Base.h>

class MainMenuState;

class MainMenuControl : public Gwen::Controls::Base
{
	GWEN_CONTROL(MainMenuControl,Gwen::Controls::Base);

public:
	void OnContinue(Gwen::Controls::Base* control);
	void OnNewGame(Gwen::Controls::Base* control);
	void OnExit(Gwen::Controls::Base* control);
};

#endif // MainMenu_h__
