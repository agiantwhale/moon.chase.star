#ifndef MAINMENU_HPP
#define MAINMENU_HPP

#include <Gwen/Controls/WindowControl.h>

class MainMenuWindowControl : public Gwen::Controls::WindowControl
{
	GWEN_CONTROL(MainMenuWindowControl,Gwen::Controls::WindowControl);

public:
	void OnContinue(Gwen::Controls::Base* control);
	void OnNewGame(Gwen::Controls::Base* control);
	void OnExit(Gwen::Controls::Base* control);
};

#endif // MainMenu_h__
