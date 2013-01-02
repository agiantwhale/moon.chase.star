#ifndef PAUSESTATE_HPP
#define PAUSESTATE_HPP

#include "../Interface/IState.hpp"
#include "../Interface/IEventListener.hpp"
#include "../GUI/PauseMenu.hpp"

class PauseState : public IState, public IEventListener
{
public:
	PauseState();
	virtual ~PauseState();

	virtual bool HandleEvent(const EventData& theevent);

	virtual void Enter(void);
	virtual bool Update(float deltaTime);
	virtual void Render(void);
	virtual void Exit(void);

private:
	PauseMenuControl* _pauseMenuControl;
	bool _endState;
};

#endif