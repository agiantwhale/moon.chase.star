#ifndef GUIEVENTDATA_HPP
#define GUIEVENTDATA_HPP

#include <Gwen/Gwen.h>
#include "../Event/EventData.hpp"

class GUIEventData : public EventData
{
public:
	GUIEventData(Gwen::Controls::Base* control) : EventData(Event_GUI), _control(control)
	{
	}

	Gwen::Controls::Base* GetControl() const { return _control; }

private:
	Gwen::Controls::Base* _control;
};

#endif