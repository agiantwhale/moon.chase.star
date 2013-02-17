#include "BackgroundTile.hpp"

namespace sb
{
	BackgroundTile::BackgroundTile( const TiXmlElement* element ) : Tile(element, sf::Vector2i(SCREENWIDTH, SCREENHEIGHT) )
	{
	}
}