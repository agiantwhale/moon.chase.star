#ifndef BodyController_h__
#define BodyController_h__

#include <SFML/Graphics/Transformable.hpp>
#include <Box2D/Box2D.h>

#include "Simulatable.hpp"

namespace sb
{
	class Entity;

	class BodyController : public Simulatable
	{
	public:
		BodyController(sf::Transformable& transformable);
		virtual ~BodyController();

		b2Body* getBody() const { return m_body; }
		void setBody(b2Body* val) { m_body = val; resetTransform(); }

	private:
		virtual void updateTransform( void );
		virtual void smoothenTransform( float remainderRatio );
		virtual void resetTransform( void );

		sf::Transformable&	m_target;
		b2Body*				m_body;
		b2Vec2				m_previousPosition;
		b2Vec2				m_smoothPosition;
		float				m_previousRotation;
		float				m_smoothRotation;
	};
}

#endif // BodyController_h__