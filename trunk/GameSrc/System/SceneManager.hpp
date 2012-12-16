#ifndef SCENEMANAGER_HPP
#define SCENEMANAGER_HPP

#include <list>
#include <vector>
#include <string>

#include "../Base/Singleton.hpp"

class SceneManager : public Singleton<SceneManager>
{
	DEFINE_SINGLETON( SceneManager );

public:
	void LoadScene( const std::string& mapName );
	void UnloadScene( void );
	inline bool IsSceneLoaded( void ) { return _sceneLoaded; }

private:
	bool _sceneLoaded;
	std::string _sceneFileName;
};

#endif