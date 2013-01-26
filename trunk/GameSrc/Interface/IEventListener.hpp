#ifndef IEventListener_HPP
#define IEventListener_HPP

#include <string>
#include "../Event/EventData.hpp"

class IEventListener
{
public:
    IEventListener(std::string handlerName = "") : _handlerName(handlerName)
    {}
    virtual ~IEventListener()
    {}
    //HandleEvent should return true if the listener "eats" the event.
    virtual bool HandleEvent(const EventData& theevent) = 0;

    inline const std::string& GetHandlerName() const
    {
        return _handlerName;
    }

	void AddEventListenType(const EventType& eventType);
	void RemoveEventListenType(const EventType& eventType);

protected:
    inline void SetHandlerName(const std::string& val)
    {
        _handlerName = val;
    }

private:
    std::string _handlerName;
};

#endif