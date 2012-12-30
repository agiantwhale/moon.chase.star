#ifndef ITASK_HPP
#define ITASK_HPP

class ITask
{
public:
	ITask(float taskDuration);
	virtual ~ITask() {}

	virtual void Start() = 0;
	//Return true when task is over.
	virtual bool DoTask(float deltaTIme);
	virtual void End() = 0;

private:
	float _timeRemaining;
};

#endif