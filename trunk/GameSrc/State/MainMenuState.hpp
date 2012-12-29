#ifndef MAINMENUSTATE_HPP
#define MAINMENUSTATE_HPP

#include "../Interface/IState.hpp"
#include "../GUI/MainMenu.hpp"

class MainMenuState : public IState
{
public:
	MainMenuState();
	virtual ~MainMenuState();

	virtual void Enter(void);
	virtual void HandleAppEvent(sf::Event& appEvent);
	virtual bool Update(float deltaTime);
	virtual void Render(void);
	virtual void Exit(void);

private:
	MainMenuWindowControl* _menuWindow;
};

#endif // MainMenuState_h__
