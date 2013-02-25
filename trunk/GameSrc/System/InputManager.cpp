#include <SFML/Graphics.hpp>
#include <Windows.h>
#include <xinput.h>
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

		if(m_inputType == kInput_Xbox)
		{
			clearVibration();
			setVibratationState();
		}
	}

	void InputManager::setUpInput( const TiXmlElement* element )
	{
		//Check for Xbox 360 Connection
		{
			XINPUT_STATE state;
			memset(&state,0, sizeof(XINPUT_STATE));
			DWORD result = XInputGetState(0, &state);
			if(result == ERROR_SUCCESS)
			{
				m_inputType = kInput_Xbox;
			}
		}
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

		/*
		{
			XINPUT_STATE state;
			memset(&state,0, sizeof(XINPUT_STATE));
			DWORD result = XInputGetState(0, &state);
			if(result == ERROR_SUCCESS)
			{
				_inputType = kInput_Xbox;
			}
		}
		*/

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
				{
					XINPUT_STATE state;
					memset(&state,0, sizeof(XINPUT_STATE));
					DWORD result = XInputGetState(0, &state);

					if(result == ERROR_SUCCESS)
					{
						if( (state.Gamepad.sThumbRX < XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE &&
							state.Gamepad.sThumbRX > -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE) )
						{
							state.Gamepad.sThumbRX = 0;
						}

						if(	(state.Gamepad.sThumbRY < XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE &&
							state.Gamepad.sThumbRY > -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE) )
						{
							state.Gamepad.sThumbRY = 0;
						}

						if(state.Gamepad.sThumbRX > 0)
						{
							m_rightInput = true;
						}
						else if(state.Gamepad.sThumbRX < 0)
						{
							m_leftInput = true;
						}

						if(state.Gamepad.sThumbRY < 0)
						{
							m_downInput = true;
						}

						if(state.Gamepad.sThumbRY > 0)
						{
							m_upInput = true;
						}
						else if(state.Gamepad.sThumbRY < 0)
						{
							m_downInput = true;
						}

						m_fallInput = state.Gamepad.wButtons & XINPUT_GAMEPAD_A;

						m_affectorRate = std::abs(state.Gamepad.sThumbRX) / 32767.0f;
					}
					else
					{
						m_inputType = kInput_Keyboard;
					}
				}

				{
					if(m_vibrateDuration.isExpired())
					{
						clearVibration();
					}

					setVibratationState();
				}

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
		if(m_inputType == kInput_Xbox)
		{
			XINPUT_VIBRATION vibration;
			memset(&vibration, 0, sizeof(XINPUT_VIBRATION));

			int leftVib = (int)(m_vibrateAmount*65535.0f);
			int rightVib = (int)(m_vibrateAmount*65535.0f);

			// Set the Vibration Values
			vibration.wLeftMotorSpeed = leftVib;
			vibration.wRightMotorSpeed = rightVib;
			// Vibrate the controller
			XInputSetState(0, &vibration);
		}
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
