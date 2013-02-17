#ifndef MainMenuState_h__
#define MainMenuState_h__

#include <SFML/Graphics.hpp>
#include "GameState.hpp"
#include "../Event/EventListener.hpp"
#include "../GUI/MainMenu.hpp"

namespace sb
{
	class MainMenuState : public sb::GameState, public sb::EventListener
	{
	public:
		MainMenuState();
		virtual ~MainMenuState();

		virtual bool handleEvent(const EventData& theevent);
		virtual void enter(void);
		virtual bool update(sf::Time deltaTime);
		virtual void render(void);
		virtual void exit(void);

	private:
		MainMenuControl* m_menuWindow;
		bool m_endState;
	};
}

#endif // MainMenuState_h__
