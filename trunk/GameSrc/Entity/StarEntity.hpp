#ifndef StarEntity_h__
#define StarEntity_h__

#include <SFML/Graphics/Sprite.hpp>
#include <Thor/Particles.hpp>
#include <Thor/Particles/Emitter.hpp>
#include <Thor/Time/StopWatch.hpp>
#include <Thor/Time/Timer.hpp>
#include "../Base/Math.hpp"
#include "Entity.hpp"
#include "../Physics/BodyController.hpp"
#include "../Helper/ScreenTranslator.hpp"

class StarEntity : public sb::Entity
{
	DEFINE_ENTITY(StarEntity,sb::Entity,'STAR')

public:
	virtual void update(sf::Time deltaTime);
	virtual bool handleEvent(const sb::EventData& theevent);
	virtual void initializeEntity( const TiXmlElement *propertyElement /* = nullptr */ );

private:
	sf::Sprite				m_starSprite;
	thor::ParticleSystem*	m_starParticle;
	sb::BodyController		m_starBody;
	sb::ScreenTranslator	m_starTranslator;

	bool					m_arrived;
	float					m_totalTime;
	sf::Time				m_arrivalTime;
	sf::Vector2f			m_previousPosition;
	sf::Vector2f			m_particleVelocity;

	sb::Spline m_xSpline;
	sb::Spline m_ySpline;

	thor::UniversalEmitter::Ptr m_emitter;
};

#endif // StarEntity_h__
