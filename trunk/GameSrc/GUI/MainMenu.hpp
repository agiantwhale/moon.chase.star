#ifndef MainMenu_h__
#define MainMenu_h__

#include <Gwen/Controls/WindowControl.h>
#include <Gwen/Controls/ListBox.h>
#include <Gwen/Controls/Base.h>
#include <map>

class LevelSelectMenuControl;

class MainMenuControl : public Gwen::Controls::Base
{
	GWEN_CONTROL(MainMenuControl,Gwen::Controls::Base);

public:
	void onContinue(Gwen::Controls::Base* control);
	void onNewGame(Gwen::Controls::Base* control);
	void onCredits(Gwen::Controls::Base* control);
	void onExit(Gwen::Controls::Base* control);

private:
	Gwen::Controls::WindowControl* m_newGameWindow;
	LevelSelectMenuControl* m_levelSelectWindow;
};

class LevelSelectMenuControl : public Gwen::Controls::WindowControl
{
	GWEN_CONTROL(LevelSelectMenuControl,Gwen::Controls::WindowControl);

public:
	void onRowSelect(Gwen::Controls::Base* control);
	void onStart(Gwen::Controls::Base* control);
	void onBack(Gwen::Controls::Base* control);

	void updateListing(void);

private:
	unsigned int m_sceneNum;
	Gwen::Controls::ListBox* m_levelListBox;
	std::map<Gwen::Controls::Base*, unsigned int> m_tableRowMap;
};

#endif // MainMenu_h__
