#include <Gwen/Controls/Base.h>
#include <Gwen/Controls/Canvas.h>
#include "CreditsState.hpp"
#include "../App/Game.hpp"
#include "../GUI/GUIManager.hpp"
#include "../Event/AppEventData.hpp"

sb::CreditsState::CreditsState() : m_creditsPage(nullptr), m_shouldEnd(false)
{
	addEventListenType(Event_App);

	m_creditsPage = new CreditsPageControl(GUIManager::getInstance()->getCanvas());
	m_creditsPage->Hide();
}

sb::CreditsState::~CreditsState()
{
	removeEventListenType(Event_App);
}

bool sb::CreditsState::handleEvent( const EventData& theevent )
{
	if( theevent.getEventType() == Event_App && isActive() )
	{
		const AppEventData& eventData = static_cast<const AppEventData&>(theevent);
		if( eventData.getAppEvent().type == sf::Event::KeyReleased )
		{
			Game::getInstance()->setNextStateType(State_MainMenu);
			m_shouldEnd = true;
		}
	}

	return false;
}

void sb::CreditsState::enter( void )
{
	GameState::enter();

	m_creditsPage->Show();
}

bool sb::CreditsState::update( sf::Time deltaTime )
{
	return m_shouldEnd;
}

void sb::CreditsState::render( void )
{
	GUIManager::getInstance()->render();
}

void sb::CreditsState::exit( void )
{
	GameState::exit();

	m_creditsPage->Hide();
}

