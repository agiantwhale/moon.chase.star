#include <SFML/Graphics.hpp>
#include "../System/InputManager.hpp"

SINGLETON_CONSTRUCTOR(InputManager),
	_inputType(kInput_Keyboard),
	_affectorRate(1.0f),
	_leftInput(false),
	_rightInput(false),
	_downInput(false)
{

}

SINGLETON_DESTRUCTOR(InputManager)
{

}

void InputManager::InitializeInput()
{
}

void InputManager::Update(float dt)
{
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