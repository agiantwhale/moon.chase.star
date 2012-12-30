#include "MainMenuState.hpp"
#include "../System/GUIManager.hpp"

MainMenuState::MainMenuState() :	IState(),
									_menuWindow(nullptr)
{
	_menuWindow = new MainMenuControl(GUIManager::GetInstance()->GetCanvas());
	_menuWindow->Hide();
}

MainMenuState::~MainMenuState()
{
	delete _menuWindow;
}

void MainMenuState::Enter( void )
{
	_menuWindow->Show();
}

bool MainMenuState::Update( float deltaTime )
{
	return false;
}

void MainMenuState::Render( void )
{
	GUIManager::GetInstance()->Render();
}

void MainMenuState::Exit( void )
{
	_menuWindow->Hide();
}

void MainMenuState::HandleAppEvent( sf::Event& appEvent )
{
	GUIManager::GetInstance()->FeedEvent(appEvent);
}

