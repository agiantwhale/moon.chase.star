#ifndef License_h__
#define License_h__

#include <Gwen/Controls/WindowControl.h>
#include <Gwen/Controls/TextBox.h>
#include <Gwen/Controls/Base.h>
#include <map>

class ActivateControl : public Gwen::Controls::WindowControl
{
	GWEN_CONTROL(ActivateControl,Gwen::Controls::WindowControl);

public:
	void onSend(Gwen::Controls::Base* control);

private:
	Gwen::Controls::TextBox* m_textBox;
};

class ConnectToInternetControl : public Gwen::Controls::WindowControl
{
	GWEN_CONTROL(ConnectToInternetControl,Gwen::Controls::WindowControl);

public:
	void onExit(Gwen::Controls::Base* control);
};

#endif // License_h__
