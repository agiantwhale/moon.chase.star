#include <SFML/Graphics/View.hpp>

#include "RenderLayer.hpp"
#include "../Helper/Globals.h"
#include "../App/Game.hpp"
#include "../Entity/PlayerEntity.hpp"
#include "../Entity/StarEntity.hpp"
#include "../Physics/PhysicsManager.hpp"
#include "../System/SceneManager.hpp"
#include "../Helper/Conversion.hpp"
#include "../Event/ContactEventData.hpp"
#include "GraphicsManager.hpp"

namespace sb
{
	void Camera::transform( void ) const
	{
		sf::View cameraView;
		cameraView.setCenter( SCREENWIDTH/2 + getPosition().x * RATIO, SCREENHEIGHT/2 - getPosition().y * RATIO);
		cameraView.setRotation(getRotation());
		cameraView.setSize(sf::Vector2f(getScale().x * SCREENWIDTH,getScale().y * SCREENHEIGHT));

		Game::getInstance()->setView(cameraView);
	}

	void RenderLayer::render( void )
	{
		m_renderCamera.transform();

		for( iterator iter = begin(); iter != end(); iter++ )
		{
			const sf::Drawable* drawable = (*iter);
			Game::getInstance()->draw(*drawable);
		}
	}

	void RenderLayer::addDrawable( const sf::Drawable& drawable )
	{
		push_back(&drawable);
	}

	void RenderLayer::removeDrawable( const sf::Drawable& drawable )
	{
		if(!empty())
		{
			remove(&drawable);
		}
	}

}