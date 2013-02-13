#ifndef EventListener_h__
#define EventListener_h__

#include "EventData.hpp"

namespace sb
{
	class EventListener
	{
	public:
		virtual ~EventListener() {}
		virtual bool handleEvent(const EventData& theevent) = 0;
		void addEventListenType(const EventType& eventType);
		void removeEventListenType(const EventType& eventType);
	};
}

#endif // EventListener_h__
