#ifndef INPUTMANAGER_HPP
#define INPUTMANAGER_HPP

#include "../Base/Singleton.hpp"
#include "../Interface/IEventListener.hpp"
#include "../Interface/IInput.hpp"

#include <list>

class InputManager : public Singleton<InputManager>, public IEventListener
{
	DEFINE_SINGLETON(InputManager)

public:
	virtual void HandleEvent(const EventData& theevent);
	virtual void ProcessInput(void);

	void SetUpInput(void);
	void AddInput(IInput* const input);
	void RemoveInput(IInput* const input);

public:
	typedef std::list<IInput*> InputList;
	typedef	InputList::iterator	InputListIterator;

	InputList _inputList;
};


#endif