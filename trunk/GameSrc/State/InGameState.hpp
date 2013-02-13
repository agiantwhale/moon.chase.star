#ifndef INGAMESTATE_HPP
#define INGAMESTATE_HPP

#include "../Interface/IState.hpp"
#include "../Interface/IEventListener.hpp"

class InGameState : public GameState, public EventListener
{
public:
    InGameState();
    virtual ~InGameState();

	virtual bool handleEvent(const EventData& theevent);

    virtual void enter(void);
    virtual bool update(float deltaTime);
    virtual void render(void);
    virtual void exit(void);

private:
	bool _endState;
};

#endif