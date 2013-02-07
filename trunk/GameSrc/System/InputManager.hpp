#ifndef INPUTMANAGER_HPP
#define INPUTMANAGER_HPP

#include "../Base/Singleton.hpp"
#include "../Interface/IEventListener.hpp"

class InputManager : public Singleton<InputManager>, public IEventListener
{
	DEFINE_SINGLETON(InputManager)

public:
	void SetUpInput(void);
	void Update(float dt);
	void FeedOutput(float amount, float duration);
	bool HandleEvent(const EventData& theevent);

	bool GetLeftInput() const { return _leftInput; }
	bool GetRightInput() const { return _rightInput; }
	bool GetDownInput() const { return _downInput; }
	float GetAffectorRate() const { return _affectorRate; }

private:
	void ClearVibration(void);
	void SetVibratationState(void);

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

	float _vibrateAmount;
	float _vibrateDuration;
};

#endif