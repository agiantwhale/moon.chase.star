#ifndef InGameState_h__
#define InGameState_h__

#include "GameState.hpp"
#include "../Event/EventListener.hpp"

namespace sb
{
	class InGameState : public GameState, public EventListener
	{
	public:
		InGameState();
		virtual ~InGameState();

		virtual bool handleEvent(const EventData& theevent);

		virtual void enter(void);
		virtual bool update(sf::Time deltaTime);
		virtual void render(void);
		virtual void exit(void);

	private:
		bool m_endState;
	};
}

#endif // InGameState_h__
