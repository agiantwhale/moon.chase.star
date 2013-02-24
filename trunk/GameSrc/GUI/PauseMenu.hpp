#ifndef PauseMenu_h__
#define PauseMenu_h__

#include <Gwen/Controls/Base.h>

class PauseState;

class PauseMenuControl : public Gwen::Controls::Base
{
	GWEN_CONTROL(PauseMenuControl,Gwen::Controls::Base);

public:
	void onButtonPressed(Gwen::Controls::Base* control);
	void onExit(Gwen::Controls::Base* control);
};

#endif // PauseMenu_h__
