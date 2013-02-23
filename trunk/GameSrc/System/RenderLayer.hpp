#ifndef RENDERLAYER_HPP
#define RENDERLAYER_HPP

#include <list>
#include <sfml/Graphics/Transformable.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include "../Event/EventListener.hpp"
#include "../Physics/BodyController.hpp"

namespace sb
{
	class Camera : public sf::Transformable
	{
	public:
		void transform( void ) const;
	};

	class RenderLayer : private std::list<const sf::Drawable*>
	{
	public:
		RenderLayer() : m_renderCamera() {}

		void render( void );
		void addDrawable( const sf::Drawable& drawable );
		void removeDrawable( const sf::Drawable& drawable );

		Camera& getCamera()
		{
			return m_renderCamera;
		}

	private:
		Camera m_renderCamera;
	};
}

#endif