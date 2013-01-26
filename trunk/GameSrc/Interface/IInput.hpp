#ifndef IINPUT_HPP
#define IINPUT_HPP

class IInput
{
public:
	IInput(unsigned int inputCode);
	virtual ~IInput();

	virtual void Begin()			= 0;
	virtual void End()				= 0;
	virtual void RegisterInput();
	virtual void UnregisterInput();

private:
	bool _started;
};

#endif