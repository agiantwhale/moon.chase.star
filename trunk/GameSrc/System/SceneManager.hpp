#ifndef SCENEMANAGER_HPP
#define SCENEMANAGER_HPP

#include <TinyXML/tinyxml.h>
#include <list>
#include <vector>
#include <string>
#include <SFML/System/Vector2.hpp>
#include <SFML/Audio.hpp>
#include <unordered_map>
#include "../Base/Singleton.hpp"
#include "../Event/EventListener.hpp"

class PlayerEntity;
class StarEntity;

class b2Body;

namespace sb
{
	class Tile;

	struct SceneInfo
	{
		std::string sceneName;
		std::string soundName;
		std::string fileName;
		std::string helperText;
	};

	class SceneManager : public Singleton<SceneManager>, private EventListener
	{
		DEFINE_SINGLETON( SceneManager );

	public:
		void setUpScene( const TiXmlElement* element );
		void saveProgress(void);
		void loadScene( unsigned int sceneNum );
		void restartScene(void);
		void unloadScene( void );
		inline bool isSceneLoaded( void )
		{
			return m_sceneLoaded;
		}

		void addTile(Tile* tile)
		{
			m_tileStack.push_back(tile);
		}

		const sf::Vector2i& getSceneSize() const { return m_levelSize; }
		unsigned int getLoadedSceneNumber() const { return m_sceneNum; }

		unsigned int getMaximumSceneNum() const { return m_sceneInfoStack.size(); }
		unsigned int getClearedSceneNum() const { return m_clearedSceneNum; }

		PlayerEntity* getPlayerEntity() const { return m_playerEntity; }
		void setPlayerEntity(PlayerEntity* val) { m_playerEntity = val; }

		StarEntity* getStarEntity() const { return m_starEntity; }
		void setStarEntity(StarEntity* val) { m_starEntity = val; }

		const std::vector<SceneInfo>& getSceneInfoStack() const { return m_sceneInfoStack; }

		const std::string& getHelperText( unsigned int sceneNum );

		void clearMusic(void)
		{
			delete m_backgroundMusic;
			m_backgroundMusic = nullptr;
		}

		const std::string& getSceneName() const { return m_sceneName; }

	private:
		virtual bool handleEvent( const EventData& newevent );

		std::string m_sceneName;
		unsigned int m_sceneNum;
		unsigned int m_clearedSceneNum;
		std::vector<SceneInfo> m_sceneInfoStack;
		bool m_gameLost;
		bool m_gameWon;
		bool m_sceneLoaded;
		sf::Vector2i m_levelSize;
		std::vector<Tile*> m_tileStack;

		sf::Music*	m_backgroundMusic;

		PlayerEntity* m_playerEntity;
		StarEntity*   m_starEntity;

		b2Body* m_zoneBody;
	};
}



#endif