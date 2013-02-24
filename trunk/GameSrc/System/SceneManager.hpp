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

		const sf::Vector2i& getLevelSize() const { return m_levelSize; }
		unsigned int getLoadedSceneNumber() const { return m_sceneNum; }

		PlayerEntity* getPlayerEntity() const { return m_playerEntity; }
		void setPlayerEntity(PlayerEntity* val) { m_playerEntity = val; }

		StarEntity* getStarEntity() const { return m_starEntity; }
		void setStarEntity(StarEntity* val) { m_starEntity = val; }

		const std::string& getHelperText( unsigned int sceneNum )
		{
			return m_helperTextStack.at(sceneNum);
		}

	private:
		virtual bool handleEvent( const EventData& newevent );

		unsigned int m_sceneNum;
		std::vector<std::string> m_sceneFileNameStack;
		std::vector<std::string> m_helperTextStack;
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