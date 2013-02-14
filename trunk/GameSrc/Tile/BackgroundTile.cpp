#include "../Tile/BackgroundTile.hpp"
#include "../System/ResourceCache.hpp"
#include "../System/EventManager.hpp"
#include "../System/SceneManager.hpp"
#include "../App/Game.hpp"

namespace sb
{
	BackgroundTile::BackgroundTile( const TiXmlElement* element ) : Tile(element, sf::Vector2i(SCREENWIDTH, SCREENHEIGHT) )
	{
	}
}