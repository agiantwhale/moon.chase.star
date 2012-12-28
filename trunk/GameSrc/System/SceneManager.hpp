#ifndef SCENEMANAGER_HPP
#define SCENEMANAGER_HPP

#include <list>
#include <vector>
#include <string>

#include "../Base/Singleton.hpp"
#include "../Interface/IEventListner.hpp"

class SceneManager : public Singleton<SceneManager>, private IEventListener
{
    DEFINE_SINGLETON( SceneManager );

public:
    void LoadScene( const std::string& mapName );
    void RestartScene( const std::string& mapName );
    void UnloadScene( void );
    inline bool IsSceneLoaded( void )
    {
        return _sceneLoaded;
    }

private:
	virtual bool HandleEvent( const EventData& newevent );

    bool _sceneLoaded;
    std::string _sceneFileName;
};

#endif