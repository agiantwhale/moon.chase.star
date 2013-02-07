#include <SFML/Graphics.hpp>
#include <Windows.h>
#include <xinput.h>
#include "../System/InputManager.hpp"

SINGLETON_CONSTRUCTOR(InputManager),
	_inputType(kInput_Keyboard),
	_affectorRate(1.0f),
	_leftInput(false),
	_rightInput(false),
	_downInput(false),
	_vibrateAmount(0.0f),
	_vibrateDuration(0.0f)
{
	AddEventListenType(Event_Unload);
}

SINGLETON_DESTRUCTOR(InputManager)
{
	RemoveEventListenType(Event_Unload);

	if(_inputType == kInput_Xbox)
	{
		ClearVibration();
		SetVibratationState();
	}
}

void InputManager::SetUpInput()
{
	//Check for Xbox 360 Connection
	{
		XINPUT_STATE state;
		memset(&state,0, sizeof(XINPUT_STATE));
		DWORD result = XInputGetState(0, &state);
		if(result == ERROR_SUCCESS)
		{
			_inputType = kInput_Xbox;
		}
	}
}

void InputManager::Update(float dt)
{
	_affectorRate = 0.0f;
	_leftInput = false;
	_rightInput = false;
	_downInput = false;

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

	switch(_inputType)
	{
	case kInput_Keyboard:
		{
			_affectorRate = 1.0f;
			_leftInput = sf::Keyboard::isKeyPressed(sf::Keyboard::Left);
			_rightInput = sf::Keyboard::isKeyPressed(sf::Keyboard::Right);
			_downInput = sf::Keyboard::isKeyPressed(sf::Keyboard::Down);
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
						_rightInput = true;
					}
					else if(state.Gamepad.sThumbRX < 0)
					{
						_leftInput = true;
					}

					if(state.Gamepad.sThumbRY < 0)
					{
						_downInput = true;
					}

					_affectorRate = std::abs(state.Gamepad.sThumbRX) / 32767.0f;
				}
				else
				{
					_inputType = kInput_Keyboard;
				}
			}

			{
				if(_vibrateDuration > 0.f)
				{
					_vibrateDuration -= dt;
					SetVibratationState();
				}
				else
				{
					ClearVibration();
					SetVibratationState();
				}
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

bool InputManager::HandleEvent( const EventData& theevent )
{
	if( theevent.GetEventType() == Event_Unload )
	{
		ClearVibration();
		return false;
	}

	return false;
}

void InputManager::SetVibratationState( void )
{
	if(_inputType == kInput_Xbox)
	{
		XINPUT_VIBRATION vibration;
		memset(&vibration, 0, sizeof(XINPUT_VIBRATION));

		int leftVib = (int)(_vibrateAmount*65535.0f);
		int rightVib = (int)(_vibrateAmount*65535.0f);

		// Set the Vibration Values
		vibration.wLeftMotorSpeed = leftVib;
		vibration.wRightMotorSpeed = rightVib;
		// Vibrate the controller
		XInputSetState(0, &vibration);
	}
}

void InputManager::ClearVibration( void )
{
	_vibrateAmount = 0.0f;
	_vibrateDuration = 0.0f;
	SetVibratationState();
}

void InputManager::FeedOutput( float amount, float duration )
{
	_vibrateAmount = amount;
	_vibrateDuration = duration;
}
