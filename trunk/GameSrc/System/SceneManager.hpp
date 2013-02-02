#ifndef SCENEMANAGER_HPP
#define SCENEMANAGER_HPP

#include <list>
#include <vector>
#include <string>
#include <SFML/Audio.hpp>
#include "../Base/Vec2D.hpp"
#include "../Base/Singleton.hpp"
#include "../Interface/IEventListener.hpp"

class Tile;

class SceneManager : public Singleton<SceneManager>, private IEventListener
{
    DEFINE_SINGLETON( SceneManager );

public:
    void LoadScene( const std::string& mapName );
    void RestartScene(void);
    void UnloadScene( void );
    inline bool IsSceneLoaded( void )
    {
        return _sceneLoaded;
    }

	void AddTile(Tile* tile)
	{
		_tileStack.push_back(tile);
	}

	const Vec2D& GetLevelSize() const { return _levelSize; }

private:
	virtual bool HandleEvent( const EventData& newevent );

    bool _sceneLoaded;
	Vec2D _levelSize;
	std::string _sceneFileName;
	std::vector<Tile*> _tileStack;
};

#endif