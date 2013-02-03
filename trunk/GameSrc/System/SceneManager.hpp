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
	void SetUpScene(void);
	void SaveProgress(void);
	void LoadScene( unsigned int sceneNum );
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
	unsigned int GetLoadedSceneNumber() const { return _sceneNum; }

private:
	virtual bool HandleEvent( const EventData& newevent );

	unsigned int _sceneNum;
	std::vector<std::string> _sceneFileNameStack;
    bool _sceneLoaded;
	Vec2D _levelSize;
	std::vector<Tile*> _tileStack;
};

#endif