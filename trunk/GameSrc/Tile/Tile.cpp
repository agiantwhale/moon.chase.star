#include "Tile.hpp"
#include "../Event/EventManager.hpp"
#include "../System/ResourceCache.hpp"
#include "../System/SceneManager.hpp"
#include "../App/Game.hpp"

namespace sb
{
	std::unordered_map<std::string,std::string> Tile::_tilesetMap;

	Tile::Tile( const TiXmlElement* element, const sf::Vector2i& tileSize )
		:	sf::Drawable(),
			m_tileTexture()
	{
		m_tileTexture.create( SceneManager::getInstance()->getSceneSize().x, SceneManager::getInstance()->getSceneSize().y );
		m_tileTexture.clear( sf::Color::Transparent );

		if(element)
		{
			const TiXmlElement *tileElement = element->FirstChildElement("tile");
			thor::ResourceKey<sf::Texture> key = thor::Resources::fromFile<sf::Texture>(_tilesetMap.find(element->Attribute("tileset"))->second);
			std::shared_ptr<sf::Texture> texture = ResourceCache::getInstance()->acquire<sf::Texture>(key);

			while(tileElement)
			{
				double xValue = 0.0, yValue = 0.0;
				tileElement->Attribute( "x", &xValue );
				tileElement->Attribute( "y", &yValue );

				int tileX = 0, tileY = 0;
				tileElement->Attribute("tx",&tileX);
				tileElement->Attribute("ty",&tileY);

				sf::Sprite tile = sf::Sprite( *texture, sf::IntRect(tileX*tileSize.x, tileY*tileSize.y, tileSize.x, tileSize.y) );
				tile.setPosition( sf::Vector2f( xValue * tileSize.x, yValue * tileSize.y ) );
				m_tileTexture.draw( tile );

				tileElement = tileElement->NextSiblingElement("tile");
			}
		}

		m_tileTexture.display();

		m_tileSprite.setTexture(m_tileTexture.getTexture());

		SceneManager::getInstance()->addTile(this);
	}

	Tile::~Tile()
	{
	}

	void sb::Tile::draw( sf::RenderTarget& target, sf::RenderStates states ) const
	{
		target.draw(m_tileSprite,states);
	}

}