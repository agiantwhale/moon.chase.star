#include "../Tile/BackgroundTile.hpp"
#include "../System/ResourceCache.hpp"
#include "../System/EventManager.hpp"
#include "../System/SceneManager.hpp"
#include "../App/Game.hpp"

void BackgroundTile::Initialize( const TiXmlElement* element )
{
	_tileTexture = new sf::RenderTexture;
	_tileTexture->create( SceneManager::GetInstance()->GetLevelSize().x, SceneManager::GetInstance()->GetLevelSize().y );
	_tileTexture->clear( sf::Color::Transparent );

	if(element)
	{
		const TiXmlElement *tileElement = element->FirstChildElement("tile");
		thor::ResourceKey<sf::Texture> key = thor::Resources::fromFile<sf::Texture>(_tilesetMap.find(element->Attribute("tileset"))->second);
		std::shared_ptr<sf::Texture> texture = ResourceCache::GetInstance()->acquire<sf::Texture>(key);

		while(tileElement)
		{
			double xValue = 0.0, yValue = 0.0;
			tileElement->Attribute( "x", &xValue );
			tileElement->Attribute( "y", &yValue );

			int tileX = 0, tileY = 0;
			tileElement->Attribute("tx",&tileX);
			tileElement->Attribute("ty",&tileY);

			sf::Sprite tile = sf::Sprite( *texture, sf::IntRect(tileX*SCREENWIDTH, tileY*SCREENHEIGHT, SCREENWIDTH, SCREENHEIGHT) );
			tile.setPosition( sf::Vector2f( xValue * SCREENWIDTH, yValue * SCREENHEIGHT ) );
			_tileTexture->draw( tile );

			tileElement = tileElement->NextSiblingElement("tile");
		}
	}

	_tileTexture->display();

	SceneManager::GetInstance()->AddTile(this);
}

