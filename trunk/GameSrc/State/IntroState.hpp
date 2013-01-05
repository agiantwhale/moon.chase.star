#ifndef INTROSTATE_HPP
#define INTROSTATE_HPP

#include <sfeMovie/Movie.hpp>
#include "../Interface/IState.hpp"

class IntroState : public IState
{
public:
    IntroState();
    virtual ~IntroState();

    virtual void Enter(void);
    virtual bool Update(float deltaTime);
    virtual void Render(void);
    virtual void Exit(void);

private:
	sfe::Movie*	_introMovie;
};

#endif