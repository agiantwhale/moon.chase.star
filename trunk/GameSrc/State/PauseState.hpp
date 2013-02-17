#ifndef PauseState_h__
#define PauseState_h__

#include "GameState.hpp"
#include "../Event/EventListener.hpp"
#include "../GUI/PauseMenu.hpp"

namespace sb
{
	class PauseState : public GameState, public EventListener
	{
	public:
		PauseState();
		virtual ~PauseState();

		virtual bool handleEvent(const EventData& theevent);

		virtual void enter(void);
		virtual bool update(sf::Time deltaTime);
		virtual void render(void);
		virtual void exit(void);

	private:
		PauseMenuControl* m_pauseMenuControl;
		bool m_endState;
	};
}

#endif // PauseState_h__
