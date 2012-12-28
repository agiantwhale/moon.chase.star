#ifndef TILE_HPP
#define TILE_HPP

#include <TinyXML\tinyxml.h>
#include <SFML\Graphics.hpp>
#include <string>
#include <unordered_map>
#include "../Interface/IRenderable.hpp"
#include "../Interface/IEventListner.hpp"

class Tile : public IRenderable, public IEventListener
{
public:
    Tile();
    virtual ~Tile();

    virtual bool HandleEvent( const EventData& newevent );

    virtual void Render(void);
    void Initialize( const TiXmlElement* element );

    static void RegisterTileset( const std::string& tile, const std::string& tileset )
    {
        _tilesetMap.insert( std::make_pair( tile, tileset ) );
    }

private:
    sf::RenderTexture* _tileTexture;

    static std::unordered_map<std::string,std::string> _tilesetMap;
};

#endif