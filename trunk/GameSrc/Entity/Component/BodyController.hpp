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

		void setBody(b2Body* body);
		b2Body* getBody();

		void setOwnerEntity(Entity *entity);
		Entity* getOwnerEntity();

	private:
		virtual void updateTransform( void );
		virtual void smoothenTransform( float remainderRatio );
		virtual void resetTransform( void );

		sf::Transformable&	m_target;
		b2Body*				m_body;
		Entity*				m_ownerEntity;

		b2Vec2				m_previousPosition;
		b2Vec2				m_smoothPosition;
		float				m_previousRotation;
		float				m_smoothRotation;
	};
}

#endif // BodyController_h__