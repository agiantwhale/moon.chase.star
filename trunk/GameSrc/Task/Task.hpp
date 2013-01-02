#ifndef TASK_HPP
#define TASK_HPP

class Task
{
public:
	Task(float taskDuration);
	virtual ~Task() {}

	void AddTask();
	void RemoveTask();

	virtual void Start();
	//Return true when task is over.
	virtual bool DoTask(float deltaTIme);
	virtual void End();

private:
	float _timeRemaining;
};

#endif