#ifndef INTROSTATE_HPP
#define INTROSTATE_HPP

#include <SFML/Graphics.hpp>
#include <Gwen/Controls/ImagePanel.h>
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
    enum IntroStatus
    {
        FADE_IN,
        WAIT,
        FADE_OUT
    };

    Gwen::Controls::ImagePanel *_splashImage;
    sf::Clock	_splashTimer;
    IntroStatus _currentStatus;
};

#endif