#ifndef SPRITEWRAPPER_HPP
#define SPRITEWRAPPER_HPP

#include <SFML/Graphics.hpp>

namespace sb
{
	class WorldSprite : public sf::Transformable
	{
	public:
		WorldSprite();
		explicit WorldSprite(const sf::Texture& texture);
		WorldSprite(const sf::Texture& texture, const sf::IntRect& rectangle);

	private:

	};
}

#endif
