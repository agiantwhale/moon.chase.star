#include "../App/Game.hpp"
#include "../System/ResourceManager.hpp"

SINGLETON_CONSTRUCTOR( TextureManager ),
                       IEventListener("TextureManager")
{
	AddEventListenType(Event_Unload);
}

SINGLETON_DESTRUCTOR( TextureManager )
{
	RemoveEventListenType(Event_Unload);
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
		TRI_LOG_STR("Failed to load resource.");
		TRI_LOG(strId);
        delete texture;
        texture = nullptr;
    }

    return texture;
}


SINGLETON_CONSTRUCTOR( SoundBufferManager ),
IEventListener("SoundBufferManager")
{
	AddEventListenType(Event_Unload);
}

SINGLETON_DESTRUCTOR( SoundBufferManager )
{
	RemoveEventListenType(Event_Unload);
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
		TRI_LOG_STR("Failed to load resource.");
		TRI_LOG(strId);
        delete buffer;
        buffer = nullptr;
    }

    return buffer;
}
