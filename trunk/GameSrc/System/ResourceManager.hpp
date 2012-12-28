#ifndef RESOURCEMANAGER_HPP
#define RESOURCEMANAGER_HPP

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <string>
#include <unordered_map>
#include "../Base/Singleton.hpp"
#include "../Interface/IEventListner.hpp"

template< class T >
class ResourceManager
{
public:
    typedef std::pair< std::string, T* >                Resource;
    typedef std::unordered_map< std::string, T* >     ResourceMap;

private:
    ResourceMap m_resource;

    T* Find( const std::string& strId )
    {
        T* resource = NULL;
        typename ResourceMap::iterator it = m_resource.find( strId );
        if( it != m_resource.end() )
        {
            resource = it->second;
        }

        return resource;
    }

protected:
    virtual T* Load( const std::string& strId ) = 0;

public:
    ResourceManager()
    {
    }

    virtual ~ResourceManager()
    {
        ReleaseAllResources();
    }

    T* GetResource( const std::string& strId )
    {
        T* resource = Find( strId );
        if( resource == NULL )
        {
            resource = Load( strId );
            // If the resource loaded successfully, add it do the resource map
            if( resource != NULL )
                m_resource.insert( Resource( strId, resource ) );
        }
        return resource;
    }

    void ReleaseResource( const std::string& strId )
    {
        T* resource = Find( strId );
        if( resource != NULL )
        {
            delete resource;
            m_resource.erase( m_resource.Find( strId ) );
        }
    }

    void ReleaseAllResources()
    {
        while( m_resource.begin() != m_resource.end() )
        {
            delete m_resource.begin()->second;
            m_resource.erase( m_resource.begin() );
        }
    }
};

class TextureManager : public Singleton<TextureManager>, public ResourceManager< sf::Texture >, private IEventListener
{
    DEFINE_SINGLETON(TextureManager)

protected:
    virtual bool HandleEvent( const EventData& newevent );
    virtual sf::Texture* Load( const std::string& strId );
};

class SoundBufferManager : public Singleton<SoundBufferManager>, public ResourceManager< sf::SoundBuffer >, private IEventListener
{
    DEFINE_SINGLETON( SoundBufferManager )

protected:
    virtual bool HandleEvent( const EventData& newevent );
    virtual sf::SoundBuffer* Load( const std::string& strId );
};

#endif // RESOURCEMANAGER_H_INCLUDED
