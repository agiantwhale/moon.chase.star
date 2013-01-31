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
	virtual bool DoTask(float deltaTime);
	virtual void End();

protected:
	float GetTimeRemaining() const { return _timeRemaining; }

private:
	float _timeRemaining;
};

#endif