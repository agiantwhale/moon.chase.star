#include "../Tile/Tile.hpp"
#include "../System/ResourceManager.hpp"
#include "../System/EventManager.hpp"
#include "../System/SceneManager.hpp"
#include "../App/Game.hpp"

std::unordered_map<std::string,std::string> Tile::_tilesetMap;

Tile::Tile() :IRenderable( nullptr ), _tileTexture(nullptr)
{
}

Tile::~Tile()
{
    delete _tileTexture;
}

void Tile::Render(void)
{
    Game::GetInstance()->draw( sf::Sprite( _tileTexture->getTexture() ) );
}

void Tile::Initialize( const TiXmlElement* element )
{
    _tileTexture = new sf::RenderTexture;
    _tileTexture->create( SceneManager::GetInstance()->GetLevelSize().x, SceneManager::GetInstance()->GetLevelSize().y );
    _tileTexture->clear( sf::Color::Transparent );

    if(element)
    {
        const TiXmlElement *tileElement = element->FirstChildElement("tile");
        const sf::Texture *tilesetSprite = TextureManager::GetInstance()->GetResource(_tilesetMap.find(element->Attribute("tileset"))->second);
        while(tileElement)
        {
            double xValue = 0.0, yValue = 0.0;
            tileElement->Attribute( "x", &xValue );
            tileElement->Attribute( "y", &yValue );

            int tileX = 0, tileY = 0;
            tileElement->Attribute("tx",&tileX);
            tileElement->Attribute("ty",&tileY);

            sf::Sprite tile = sf::Sprite( *tilesetSprite, sf::IntRect(tileX*TILESIZE, tileY*TILESIZE, TILESIZE, TILESIZE) );
            tile.setPosition( sf::Vector2f( xValue * TILESIZE, yValue * TILESIZE ) );
            _tileTexture->draw( tile );

            tileElement = tileElement->NextSiblingElement("tile");
        }
    }

    _tileTexture->display();

	SceneManager::GetInstance()->AddTile(this);
}