#ifndef BACKGROUNDTILE_HPP
#define BACKGROUNDTILE_HPP

#include "Tile.hpp"

namespace sb
{
	class BackgroundTile : public Tile
	{
	public:
		BackgroundTile( const TiXmlElement* element );
	};
}

#endif