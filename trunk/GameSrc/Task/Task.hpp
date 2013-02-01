#ifndef TASK_HPP
#define TASK_HPP

class TaskManager;

class Task
{
public:
	enum TaskState
	{
		kTask_Unregistered,
		kTask_Registered,
		kTask_Started,
		kTask_Ended
	};

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
	TaskState	_taskState;

	friend class TaskManager;
};

#endif