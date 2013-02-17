#ifndef GUIEventData_h__
#define GUIEventData_h__

#include <Gwen/Gwen.h>
#include "EventData.hpp"

namespace sb
{
	class GUIEventData : public EventData
	{
	public:
		GUIEventData(Gwen::Controls::Base* control) : EventData(Event_GUI), m_control(control)
		{
		}

		Gwen::Controls::Base* getControl() const { return m_control; }

	private:
		Gwen::Controls::Base* m_control;
	};
}


#endif // GUIEventData_h__
