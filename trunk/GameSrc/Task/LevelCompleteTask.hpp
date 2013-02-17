#ifndef LEVELCOMPLETETASK_HPP
#define LEVELCOMPLETETASK_HPP

#include <vector>
#include <SFML/System/Vector2.hpp>
#include "../Task/Task.hpp"
#include "../Base/Math.hpp"

typedef std::vector<sf::Vector2f> VectorStack;

class PlayerEntity;

class LevelCompleteTask : public sb::Task
{
public:
	LevelCompleteTask(PlayerEntity* playerEntity, const VectorStack& path);

	virtual void start();
	virtual bool doTask(sf::Time deltaTime);
	virtual void end();

private:
	PlayerEntity* m_playerEntity;
	VectorStack m_path;

	float m_totalTime;
	sb::Spline m_xSpline;
	sb::Spline m_ySpline;

	enum
	{
		kLevelComplete_Spline,
		kLevelComplete_Arrived
	} m_currentState;
};

#endif