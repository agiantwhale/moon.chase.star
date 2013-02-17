#ifndef AppEventData_h__
#define AppEventData_h__

#include <SFML/Window/Event.hpp>
#include "EventData.hpp"

namespace sb
{
	class AppEventData : public EventData
	{
	public:
		AppEventData(sf::Event& appEvent) : EventData(Event_App), m_appEvent(appEvent)
		{
		}

		sf::Event& getAppEvent(void) const {return m_appEvent;}

	private:
		sf::Event& m_appEvent;
	};
}


#endif // AppEventData_h__
