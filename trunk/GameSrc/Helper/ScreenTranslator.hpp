#ifndef ScreenPositioner_h__
#define ScreenPositioner_h__

#include <SFML/Graphics/Transformable.hpp>
#include "../Base/Globals.hpp"

namespace sb
{
	namespace Translate
	{
		//************************************
		// Method:    Position
		// FullName:  sb::Translate::Position
		// Access:    public 
		// Returns:   sf::Vector2f
		// Qualifier:
		// Parameter: const sf::Vector2f worldPosition
		//************************************
		inline sf::Vector2f Position(const sf::Vector2f worldPosition)
		{
			sf::Vector2f screenPosition = sf::Vector2f( SCREENWIDTH/2, SCREENHEIGHT/2 );
			screenPosition.x += worldPosition.x * RATIO;
			screenPosition.y -= worldPosition.y * RATIO;

			return screenPosition;
		}

		//************************************
		// Method:    Velocity
		// FullName:  sb::Translate::Velocity
		// Access:    public 
		// Returns:   sf::Vector2f
		// Qualifier:
		// Parameter: const sf::Vector2f worldVelocity
		//************************************
		inline sf::Vector2f Velocity(const sf::Vector2f worldVelocity)
		{
			sf::Vector2f screenVelocity = worldVelocity;
			screenVelocity.x *= -1.f;
			screenVelocity *= RATIO;

			return screenVelocity;
		}
	}

	class ScreenTranslator
	{
	public:
		ScreenTranslator(const sf::Transformable& parent) : m_transformable(parent)
		{}

		void translate(sf::Transformable& positionable)
		{
			positionable.setPosition(Translate::Position(m_transformable.getPosition()));
			positionable.setScale(m_transformable.getScale());
			positionable.setRotation(m_transformable.getRotation());
		}

	private:
		const sf::Transformable& m_transformable;
	};
}

#endif // ScreenPositioner_h__
