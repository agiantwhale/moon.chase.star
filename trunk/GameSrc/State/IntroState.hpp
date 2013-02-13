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
    virtual bool update(float deltaTime);
    virtual void render(void);
    virtual void exit(void);

private:
	sftheora::Video _introVideo;
};

#endif