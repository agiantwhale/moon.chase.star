#ifndef APPEVENTDATA_HPP
#define APPEVENTDATA_HPP

#include <SFML/Graphics.hpp>
#include "../Event/EventData.hpp"

class AppEventData : public EventData
{
public:
	AppEventData(sf::Event& appEvent) : EventData(Event_App), _appEvent(appEvent)
	{
	}

	sf::Event& GetAppEvent(void) const {return _appEvent;}

private:
	sf::Event& _appEvent;
};

#endif