#ifndef INPUTMANAGER_HPP
#define INPUTMANAGER_HPP

#include "../Base/Singleton.hpp"

class InputManager : public Singleton<InputManager>
{
	DEFINE_SINGLETON(InputManager)

public:
	void InitializeInput(void);
	void Update(float dt);

	bool GetLeftInput() const { return _leftInput; }
	bool GetRightInput() const { return _rightInput; }
	bool GetDownInput() const { return _downInput; }
	float GetAffectorRate() const { return _affectorRate; }

private:
	enum InputType
	{
		kInput_Keyboard,
		kInput_Xbox,
		kInput_Kinect,
		kInput_LEAP
	} _inputType;

	float _affectorRate;
	bool  _leftInput;
	bool  _rightInput;
	bool  _downInput;
};

#endif