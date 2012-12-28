#include "../App/Game.hpp"
#include "../System/ResourceManager.hpp"

SINGLETON_CONSTRUCTOR( TextureManager ),
                       IEventListener("TextureManager")
{
}

SINGLETON_DESTRUCTOR( TextureManager )
{
}

bool TextureManager::HandleEvent( const EventData& newevent )
{
    if( newevent.GetEventType() == Event_Unload )
    {
        ReleaseAllResources();
        return false;
    }

    return false;
}

sf::Texture* TextureManager::Load( const std::string& strId )
{
    sf::Texture* texture = new sf::Texture;
    if( !texture->loadFromFile( strId ) )
    {
        LOG(WARNING) << "TextureManager failed to load " << strId << ".";
        delete texture;
        texture = NULL;
    }

    return texture;
}


SINGLETON_CONSTRUCTOR( SoundBufferManager ),
IEventListener("SoundBufferManager")
{
}

SINGLETON_DESTRUCTOR( SoundBufferManager )
{
}

bool SoundBufferManager::HandleEvent( const EventData& newevent )
{
    if( newevent.GetEventType() == Event_Unload )
    {
        ReleaseAllResources();
        return false;
    }

    return false;
}

sf::SoundBuffer* SoundBufferManager::Load( const std::string& strId )
{
    sf::SoundBuffer* buffer = new sf::SoundBuffer();
    if( !buffer->loadFromFile( strId ) )
    {
        LOG(WARNING) << "SoundBufferManager failed to load " << strId << ".";
        delete buffer;
        buffer = NULL;
    }

    return buffer;
}
