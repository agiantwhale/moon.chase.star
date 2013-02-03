#ifndef BACKGROUNDTILE_HPP
#define BACKGROUNDTILE_HPP

#include "../Tile/Tile.hpp"

class BackgroundTile : public Tile
{
public:
	void Initialize( const TiXmlElement* element );
};

#endif