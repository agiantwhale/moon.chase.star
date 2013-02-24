#ifndef GameWinState_h__
#define GameWinState_h__

#include "GameState.hpp"
#include "../Event/EventListener.hpp"
#include "../GUI/GameWinMenu.hpp"

namespace sb
{
	class GameWinState : public GameState, public EventListener
	{
	public:
		GameWinState();
		virtual ~GameWinState();

		virtual bool handleEvent(const EventData& theevent);
		virtual void enter(void);
		virtual bool update(sf::Time deltaTime);
		virtual void render(void);
		virtual void exit(void);

	private:
		bool m_endState;
		GameWinMenuControl*	m_gameWinMenuControl;
	};
}

#endif // GameWinState_h__
