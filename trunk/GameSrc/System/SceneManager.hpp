#ifndef SCENEMANAGER_HPP
#define SCENEMANAGER_HPP

#include <list>
#include <vector>
#include <string>

#include "../Base/Singleton.hpp"
#include "../Interface/IEventListner.hpp"

class SceneManager : public Singleton<SceneManager>, public IEventListener
{
	DEFINE_SINGLETON( SceneManager );

public:
	virtual const std::string& GetEventListenerName( void ) { return "SceneManager"; }
	virtual bool HandleEvent( const EventData& newevent );

	void LoadScene( const std::string& mapName );
	void RestartScene( const std::string& mapName );
	void UnloadScene( void );
	inline bool IsSceneLoaded( void ) { return _sceneLoaded; }

private:
	bool _sceneLoaded;
	std::string _sceneFileName;
};

#endif