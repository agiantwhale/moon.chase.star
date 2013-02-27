#include <SFML/Graphics.hpp>
#include "InputManager.hpp"
#include "../Event/AppEventData.hpp"

namespace sb
{
	SINGLETON_CONSTRUCTOR(InputManager),
		m_inputType(kInput_Keyboard),
		m_affectorRate(1.0f),
		m_leftInput(false),
		m_rightInput(false),
		m_downInput(false),
		m_upInput(false),
		m_fallInput(false),
		m_waitFrame(false),
		m_vibrateAmount(0.0f),
		m_vibrateDuration()
	{
		addEventListenType(Event_App);
		addEventListenType(Event_Unload);
	}

	SINGLETON_DESTRUCTOR(InputManager)
	{
		addEventListenType(Event_App);
		removeEventListenType(Event_Unload);
	}

	void InputManager::setUpInput( const TiXmlElement* element )
	{
	}

	void InputManager::update(sf::Time deltaTime)
	{
		m_affectorRate = 0.0f;
		m_leftInput = false;
		m_rightInput = false;
		m_downInput = false;
		m_upInput = false;

		if(!m_waitFrame)
		{
			m_fallInput = false;
		}
		else
		{
			m_waitFrame = false;
		}

		switch(m_inputType)
		{
		case kInput_Keyboard:
			{
				m_affectorRate = 1.0f;
				m_leftInput = sf::Keyboard::isKeyPressed(sf::Keyboard::Left);
				m_rightInput = sf::Keyboard::isKeyPressed(sf::Keyboard::Right);
				m_downInput = sf::Keyboard::isKeyPressed(sf::Keyboard::Down);
				m_upInput = sf::Keyboard::isKeyPressed(sf::Keyboard::Up);
				break;
			}

		case kInput_Xbox:
			{
				break;
			}

		case kInput_Kinect:
			{
				break;
			}

		case kInput_LEAP:
			{
				break;
			}
		}
	}

	bool InputManager::handleEvent( const EventData& theevent )
	{
		if( theevent.getEventType() == Event_Unload )
		{
			clearVibration();
			setVibratationState();
		}

		if( theevent.getEventType() == Event_App )
		{
			const AppEventData& eventData = static_cast<const AppEventData&>(theevent);
			if( eventData.getAppEvent().type == sf::Event::KeyReleased && eventData.getAppEvent().key.code == sf::Keyboard::Space )
			{
				m_fallInput = true;
				m_waitFrame = true;
			}
		}

		return false;
	}

	void InputManager::setVibratationState( void )
	{
	}

	void InputManager::clearVibration( void )
	{
		m_vibrateAmount = 0.0f;
		m_vibrateDuration.stop();
	}

	void InputManager::feedOutput( float amount, sf::Time duration )
	{
		m_vibrateAmount = amount;
		m_vibrateDuration.restart(duration);
	}
}
