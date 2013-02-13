#ifndef INTROSTATE_HPP
#define INTROSTATE_HPP

#include <sfTheora.h>
#include "../Interface/IState.hpp"

class IntroState : public GameState
{
public:
    IntroState();
    virtual ~IntroState();

    virtual void enter(void);
    virtual bool Update(float deltaTime);
    virtual void Render(void);
    virtual void Exit(void);

private:
	sftheora::Video _introVideo;
};

#endif