#ifndef CreditsState_h__
#define CreditsState_h__

#include "GameState.hpp"
#include "../Event/EventListener.hpp"
#include "../GUI/CreditsPage.hpp"

namespace sb
{
	class CreditsState : public GameState, public EventListener
	{
	public:
		CreditsState();
		virtual ~CreditsState();

		virtual bool handleEvent(const EventData& theevent);

		virtual void enter(void);
		virtual bool update(sf::Time deltaTime);
		virtual void render(void);
		virtual void exit(void);

	private:
		CreditsPageControl* m_creditsPage;
		bool m_shouldEnd;
	};
}

#endif // CreditsState_h__
