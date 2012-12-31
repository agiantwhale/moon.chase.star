#ifndef MAINMENUSTATE_HPP
#define MAINMENUSTATE_HPP

#include <SFML/Graphics.hpp>
#include "../Interface/IState.hpp"
#include "../Interface/IEventListener.hpp"
#include "../GUI/MainMenu.hpp"

class MainMenuState : public IState, public IEventListener
{
public:
	MainMenuState();
	virtual ~MainMenuState();

	virtual bool HandleEvent(const EventData& theevent);
	virtual void Enter(void);
	virtual void HandleAppEvent(sf::Event& appEvent);
	virtual bool Update(float deltaTime);
	virtual void Render(void);
	virtual void Exit(void);

private:
	MainMenuControl* _menuWindow;
	bool _endState;
};

#endif // MainMenuState_h__
