#ifndef WorldDrawable_h__
#define WorldDrawable_h__

#include <SFML/Graphics/Transformable.hpp>
#include <SFML/Graphics/Drawable.hpp>

#include "../System/GraphicsManager.hpp"

namespace sb
{
	template < class T >
	class Positioner
	{

	};

	template < class T >
	class WorldDrawable : public sf::Transformable, public sf::Drawable
	{
	public:
		WorldDrawable();

		T& getDrawable() const { return m_drawable; }
		void setDrawable(const T& val) { m_drawable = val; }

		void registerDrawable(unsigned int layer)
		{
			GraphicsManager::getInstance()->addDrawable(*this,layer);
		}

		void unregisterDrawable()
		{
			GraphicsManager::getInstance()->removeDrawable(*this,layer);
		}

	private:
		virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

		T	m_drawable;
		unsigned int m_layer;
	};

	template < class T >
	sb::WorldDrawable<T>::WorldDrawable() : sf::Transformable, sf::Drawable, m_drawable(), m_layer(0)
	{

	}

	template < class T >
	void WorldDrawable<T>::draw( sf::RenderTarget& target, sf::RenderStates states ) const
	{
		sf::Vector2f worldVector = getPosition();
		sf::Vector2f screenVector( SCREENWIDTH/2, SCREENHEIGHT/2 );

		screenVector.x += worldVector.x * RATIO;
		screenVector.y += worldVector.y * RATIO;

		m_drawable.setPosition(screenVector);
		m_drawable.setScale(getScale());
		m_drawable.setRotation(getRotation());

		target.draw(m_drawable, states);
	}
}


#endif // WorldDrawable_h__
