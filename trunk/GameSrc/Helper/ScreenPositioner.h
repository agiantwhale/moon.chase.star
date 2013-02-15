#ifndef ScreenPositioner_h__
#define ScreenPositioner_h__

#include <SFML/Graphics/Transformable.hpp>

namespace sb
{
	class ScreenPositioner
	{
	public:
		ScreenPositioner(const sf::Transformable& parent) : m_transformable(parent)
		{}

		void arrange(sf::Transformable& positionable)
		{
			sf::Vector2f worldVector = m_transformable.getPosition();
			sf::Vector2f screenVector( SCREENWIDTH/2, SCREENHEIGHT/2 );

			screenVector.x += worldVector.x * RATIO;
			screenVector.y += worldVector.y * RATIO;

			positionable.setPosition(screenVector);
			positionable.setScale(m_transformable.getScale());
			positionable.setRotation(m_transformable.getRotation());
		}

	private:
		const sf::Transformable& m_transformable;
	};
}

#endif // ScreenPositioner_h__
