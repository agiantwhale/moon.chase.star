#ifndef MovingPlatformEntity_h__
#define MovingPlatformEntity_h__

#include <SFML/Graphics/Sprite.hpp>
#include <Thor/Time/Timer.hpp>
#include "Entity.hpp"
#include "../Physics/BodyController.hpp"
#include "../Helper/ScreenTranslator.hpp"
#include <vector>

class MovingPlatformEntity : public sb::Entity
{
	DEFINE_ENTITY(MovingPlatformEntity,sb::Entity,'MVPT')

private:
	virtual void update(sf::Time deltaTime);
	virtual void initializeEntity( const TiXmlElement *propertyElement /* = nullptr */ );

	//b2Body* getPlatformBody() const { return m_platformBody.getBody(); }

	const sf::Vector2f& getNextPlatformDestination();

	sf::Sprite				m_platformSprite;
	sb::BodyController		m_platformBody;
	sb::ScreenTranslator	m_platformScreenTranslator;

	std::vector<sf::Vector2f>			m_platformRoute;
	std::vector<sf::Vector2f>::iterator	m_platformRouteIterator;
	float								m_travelSpeed;
	thor::Timer							m_moveTimer;
};

#endif // MovingPlatformEntity_h__
