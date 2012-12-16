#ifndef IEVENTLISTNER_HPP
#define IEVENTLISTNER_HPP

#include <string>
#include "../Event/EventData.hpp"

class IEventListener
{
public:
	IEventListener()
	{}
	virtual ~IEventListener()
	{}
	virtual const std::string& GetEventListenerName() = 0;
	virtual bool HandleEvent(const EventData& theevent) = 0;
};

#endif