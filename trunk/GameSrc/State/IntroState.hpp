#ifndef INTROSTATE_HPP
#define INTROSTATE_HPP

#include <SFML\Graphics.hpp>
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

public:
	enum IntroStatus
	{
		FADE_IN,
		WAIT,
		FADE_OUT
	};

	sf::Sprite *_splashSprite;
	sf::Clock	_splashTimer;
	IntroStatus _currentStatus;
};

#endif