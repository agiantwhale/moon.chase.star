#ifndef EventData_h__
#define EventData_h__

#include "EventsDef.h"

namespace sb
{
	class EventData
	{
	public:
		EventData(EventType type);
		virtual ~EventData();

		inline const EventType& getEventType() const
		{
			return m_eventType;
		}

		void triggerEvent(void);
		void queueEvent( sf::Time waitTime = sf::Time::Zero );
		void abortEvent(bool allOfType = false);

	private:
		EventType m_eventType;
	};
}


#endif // EventData_h__
