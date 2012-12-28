#ifndef INGAMESTATE_HPP
#define INGAMESTATE_HPP

#include "../Interface/IState.hpp"

class InGameState : public IState
{
public:
    InGameState();
    virtual ~InGameState();

    virtual void Enter(void);
    virtual bool Update(float deltaTime);
    virtual void Render(void);
    virtual void Exit(void);
};

#endif