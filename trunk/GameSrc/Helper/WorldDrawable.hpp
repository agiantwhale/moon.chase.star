#ifndef WorldDrawable_h__
#define WorldDrawable_h__

#include <SFML/Graphics/Transformable.hpp>
#include <SFML/Graphics/Drawable.hpp>

namespace sb
{
	template < class T >
	class WorldDrawable : public sf::Transformable, public sf::Drawable
	{
	public:
		WorldDrawable(const T& drawable);

	private:
		virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

		const T&	m_drawable;
	};

	template < class T >
	WorldDrawable<T>::WorldDrawable( const T& drawable ) : sf::Transformable(), sf::Drawable(), m_drawable(drawable)
	{

	}

	template < class T >
	void WorldDrawable<T>::draw( sf::RenderTarget& target, sf::RenderStates states ) const
	{
		sf::Vector2f worldVector = getPosition();
		sf::Vector2f screenVector( SCREENWIDTH/2, SCREENHEIGHT/2 );

		screenVector.x += worldVector.x * RATIO;
		screenVector.y += worldVector.y * RATIO;

		m_drawable->setPosition(screenVector);
		m_drawable->setScale(getScale());
		m_drawable->setRotation(getRotation());

		target.draw(m_drawable, states);
	}
}


#endif // WorldDrawable_h__
