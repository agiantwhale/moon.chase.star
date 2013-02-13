#ifndef INGAMESTATE_HPP
#define INGAMESTATE_HPP

#include "../Interface/IState.hpp"
#include "../Interface/IEventListener.hpp"

class InGameState : public GameState, public IEventListener
{
public:
    InGameState();
    virtual ~InGameState();

	virtual bool HandleEvent(const EventData& theevent);

    virtual void enter(void);
    virtual bool Update(float deltaTime);
    virtual void Render(void);
    virtual void Exit(void);

private:
	bool _endState;
};

#endif