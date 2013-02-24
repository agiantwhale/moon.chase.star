#ifndef GameWinMenu_h__
#define GameWinMenu_h__

#include <Gwen/Controls/Base.h>
#include "Gwen/Controls/Label.h"

class GameWinMenuControl : public Gwen::Controls::Base
{
	GWEN_CONTROL(GameWinMenuControl,Gwen::Controls::Base);

public:
	void onButtonPressed(Gwen::Controls::Base* control);
	void onExit(Gwen::Controls::Base* control);

	void setLevelName( const std::string& levelName );

private:
	Gwen::Controls::Label* m_levelName;
};

#endif // GameWinMenu_h__
