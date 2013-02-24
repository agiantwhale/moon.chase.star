#ifndef TILE_HPP
#define TILE_HPP

#include <TinyXML\tinyxml.h>
#include <SFML\Graphics.hpp>
#include <string>
#include <unordered_map>
#include "../Base/Globals.hpp"

namespace sb
{
	class Tile : public sf::Drawable
	{
	public:
		Tile( const TiXmlElement* element, const sf::Vector2i& tileSize = sf::Vector2i(TILESIZE,TILESIZE), unsigned int layer = 1 );
		virtual ~Tile();

		static void registerTileset( const std::string& tile, const std::string& tileset )
		{
			_tilesetMap.insert( std::make_pair( tile, tileset ) );
		}

		unsigned int getLayer() const { return m_layer; }

	private:
		virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

		sf::RenderTexture	m_tileTexture;
		sf::Sprite			m_tileSprite;
		unsigned int		m_layer;

		static std::unordered_map<std::string,std::string> _tilesetMap;
	};
}

#endif