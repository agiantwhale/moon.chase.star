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
		Tile( const TiXmlElement* element, const sf::Vector2i& tileSize = sf::Vector2i(TILESIZE,TILESIZE) );
		virtual ~Tile();

		static void registerTileset( const std::string& tile, const std::string& tileset )
		{
			_tilesetMap.insert( std::make_pair( tile, tileset ) );
		}

	private:
		virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

		sf::RenderTexture* m_tileTexture;

		static std::unordered_map<std::string,std::string> _tilesetMap;
	};
}

#endif