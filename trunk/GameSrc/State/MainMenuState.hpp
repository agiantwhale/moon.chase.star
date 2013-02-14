#ifndef MAINMENUSTATE_HPP
#define MAINMENUSTATE_HPP

#include <SFML/Graphics.hpp>
#include "../Interface/IState.hpp"
#include "../Interface/IEventListener.hpp"
#include "../GUI/MainMenu.hpp"

class MainMenuState : public GameState, public EventListener
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
	MainMenuControl* _menuWindow;
	bool _endState;
};

#endif // MainMenuState_h__
