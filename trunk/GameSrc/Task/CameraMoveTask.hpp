#ifndef CameraMoveTask_h__
#define CameraMoveTask_h__

#include <SFML/System/Vector2.hpp>
#include "Task.hpp"

class Camera;

class CameraMoveTask : public sb::Task
{
public:
	CameraMoveTask( const sf::Vector2f& moveDistance, unsigned int renderLayer, float affector );

	virtual void start();
	virtual bool doTask(sf::Time deltaTime);
	virtual void end();

private:
	unsigned int m_renderLayer;
	sf::Vector2f m_initialPosition;
	sf::Vector2f m_finalDestination;
	sf::Vector2f m_moveSpeed;
	float			m_affector;
};

#endif // CameraMoveTask_h__
