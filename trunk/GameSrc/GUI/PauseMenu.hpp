#ifndef PAUSEMENU_HPP
#define PAUSEMENU_HPP

#include <Gwen/Controls/Base.h>

class PauseState;

class PauseMenuControl : public Gwen::Controls::Base
{
	GWEN_CONTROL(PauseMenuControl,Gwen::Controls::Base);

public:
	void onButtonPressed(Gwen::Controls::Base* control);
	void onExit(Gwen::Controls::Base* control);
};

#endif