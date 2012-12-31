#ifndef APPEVENTEVENTDATA_HPP
#define APPEVENTEVENTDATA_HPP

#include <SFML/Graphics.hpp>
#include "../Event/EventData.hpp"

class AppEventEventData : public EventData
{
public:
	AppEventEventData(sf::Event& appEvent) : EventData(Event_AppEvent), _appEvent(appEvent)
	{
	}

	sf::Event& GetAppEvent(void) const {return _appEvent;}

private:
	sf::Event& _appEvent;
};

#endif