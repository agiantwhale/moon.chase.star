#ifndef SCENEMANAGER_HPP
#define SCENEMANAGER_HPP

#include <list>
#include <vector>
#include <string>
#include <SFML/Audio.hpp>
#include <unordered_map>
#include "../Base/Vec2D.hpp"
#include "../Base/Singleton.hpp"
#include "../Interface/IEventListener.hpp"
#include "../Interface/ITransform.hpp"

class Tile;
class PlayerEntity;
class StarEntity;

class SceneManager : public Singleton<SceneManager>, private EventListener
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

	PlayerEntity* GetPlayerEntity() const { return _playerEntity; }
	void SetPlayerEntity(PlayerEntity* val) { _playerEntity = val; }

	StarEntity* GetStarEntity() const { return _starEntity; }
	void SetStarEntity(StarEntity* val) { _starEntity = val; }

private:
	virtual bool HandleEvent( const EventData& newevent );

	unsigned int _sceneNum;
	std::vector<std::string> _sceneFileNameStack;
    bool _sceneLoaded;
	Vec2D _levelSize;
	std::vector<Tile*> _tileStack;

	sf::Music*	_backgroundMusic;

	PlayerEntity* _playerEntity;
	StarEntity*   _starEntity;
};

#endif