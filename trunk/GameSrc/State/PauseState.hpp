#ifndef PAUSESTATE_HPP
#define PAUSESTATE_HPP

#include "../Interface/IState.hpp"
#include "../Interface/IEventListener.hpp"
#include "../GUI/PauseMenu.hpp"

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
	PauseMenuControl* _pauseMenuControl;
	bool _endState;
};

#endif