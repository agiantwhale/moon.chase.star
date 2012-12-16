#ifndef EVENTMANAGER_HPP
#define EVENTMANAGER_HPP

#include <list>
#include <unordered_map>

#include "../Base/Singleton.hpp"
#include "../Event/EventData.hpp"

class EventManager : public Singleton<EventManager>
{
	DEFINE_SINGLETON( EventManager )

public:

};

#endif