#ifndef PAUSEMENU_HPP
#define PAUSEMENU_HPP

#include <Gwen/Controls/Base.h>

class PauseState;

class PauseMenuControl : public Gwen::Controls::Base
{
	GWEN_CONTROL(PauseMenuControl,Gwen::Controls::Base);

public:
	void OnButtonPressed(Gwen::Controls::Base* control);
	void OnExit(Gwen::Controls::Base* control);
};

#endif