#include <string>
#include <CxxTL\tri_logger.hpp>
#include <TinyXML\tinyxml.h>

#include "SceneManager.hpp"
#include "EntityManager.hpp"
#include "GraphicsManager.hpp"

#include "../App/Game.hpp"
#include "../Base/Globals.hpp"
#include "../Entity/Entity.hpp"
#include "../Tile/Tile.hpp"
#include "../Tile/BackgroundTile.hpp"
#include "../Entity/ZoneEntity.hpp"

namespace sb
{
	SINGLETON_CONSTRUCTOR(SceneManager),
						  EventListener(),
						  m_levelSize(),
						  m_sceneLoaded(false),
						  m_sceneNum(0),
						  m_backgroundMusic(nullptr)
	{
		addEventListenType(Event_Unload);
	}

	SINGLETON_DESTRUCTOR(SceneManager)
	{
		removeEventListenType(Event_Unload);
	}

	bool SceneManager::handleEvent( const EventData& newevent )
	{
		if( newevent.getEventType() == Event_Unload )
		{
			unloadScene();
		}

		return false;
	}

	void SceneManager::restartScene(void)
	{
		m_playerEntity = nullptr;
		m_starEntity = nullptr;

		TRI_LOG_STR("Restarting scene.");

		TiXmlDocument sceneDocument;

		if( !sceneDocument.LoadFile(m_sceneFileNameStack[m_sceneNum]) )
		{
			TRI_LOG_STR("Unable to load scene.");
			TRI_LOG(m_sceneNum);
			return;
		}

		EntityFactory* entityFcty = EntityFactory::getInstance();
		EntityManager* entityMgr = EntityManager::getInstance();

		TiXmlElement *levelElement = sceneDocument.FirstChildElement( "level" );
		if( !levelElement )
		{
			TRI_LOG_STR("Wrong scene structure.");
		}
		else
		{
			TiXmlElement *objectsElement = levelElement->FirstChildElement( "Objects" );
			if( objectsElement )
			{
				TiXmlElement *objectElement = objectsElement->FirstChildElement();
				while( objectElement )
				{
					Entity *objectEntity = entityFcty->createEntity(objectElement->ValueStr());
					if( objectEntity )
					{
						objectEntity->initializeEntity( objectElement );
						entityMgr->registerEntity(objectEntity);
					}
					objectElement = objectElement->NextSiblingElement();
				}
			}
		}

		{
			const int horizontalBlocks = m_levelSize.x / SCREENWIDTH;
			const int verticalBlocks = m_levelSize.y / SCREENHEIGHT;

			for(int h = 0; h < horizontalBlocks; h++)
			{
				for(int v = 0; v < verticalBlocks; v++)
				{
					sf::Vector2f worldPosition(h * SCREENWIDTH * UNRATIO, v * SCREENHEIGHT * UNRATIO * -1.f );
					Entity* entity = new ZoneEntity;
					entity->setPosition(worldPosition);
					entity->initializeEntity();
					entityMgr->registerEntity(entity);
				}
			}
		}

		entityMgr->postLoad();

		TRI_LOG_STR("Load complete.");

		m_sceneLoaded = true;
	}

	void SceneManager::loadScene( unsigned int sceneNum )
	{
		if(sceneNum >= m_sceneFileNameStack.size())
		{
			sceneNum = 0;
		}
		std::string sceneFileName = m_sceneFileNameStack.at(sceneNum);

		Game::getInstance()->pauseFrameTimer();

		TRI_LOG_STR("Loading scene.");

		TiXmlDocument sceneDocument;

		if( !sceneDocument.LoadFile(sceneFileName) )
		{
			TRI_LOG_STR("Unable to load scene.");
			TRI_LOG(m_sceneNum);
			return;
		}
		else
		{
			m_sceneNum = sceneNum;
		}

		EntityFactory* entityFcty = EntityFactory::getInstance();
		EntityManager* entityMgr = EntityManager::getInstance();

		TiXmlElement *levelElement = sceneDocument.FirstChildElement( "level" );
		if( !levelElement )
		{
			TRI_LOG_STR("Wrong scene structure.");
		}
		else
		{
			//Initialize all this later, after loading all crap from the scene file.
			int levelWidth = 0.0f, levelHeight = 0.0f;
			levelElement->QueryIntAttribute("width",&levelWidth);
			levelElement->QueryIntAttribute("height",&levelHeight);
			m_levelSize.x = levelWidth;
			m_levelSize.y = levelHeight;

			std::string musicFileName;
			levelElement->QueryStringAttribute("BackgroundMusic", &musicFileName);
			if(!musicFileName.empty())
			{
				m_backgroundMusic = new sf::Music();
				m_backgroundMusic->openFromFile(musicFileName);
				m_backgroundMusic->setLoop(true);
				m_backgroundMusic->play();
			}

			//Collision
			TiXmlElement *collisionElement = levelElement->FirstChildElement("Collision");
			if(collisionElement)
			{
				TiXmlElement *hullElement = collisionElement->FirstChildElement("rect");
				while(hullElement)
				{
					Entity *hullEntity = entityFcty->createEntity("Hull");
					if(hullEntity)
					{
						hullEntity->initializeEntity(hullElement);
						entityMgr->registerEntity( hullEntity );
					}
					hullElement = hullElement->NextSiblingElement();
				}
			}

			//Tiles
			TiXmlElement *tilesElement = levelElement->FirstChildElement("Tiles");
			if(tilesElement)
			{
				Tile* tile = new Tile(tilesElement);
				GraphicsManager::getInstance()->addDrawable(tile,1);
			}

			//Tiles
			TiXmlElement *backgroundTilesElement = levelElement->FirstChildElement("Backgrounds");
			if(backgroundTilesElement)
			{
				BackgroundTile* tile = new BackgroundTile( backgroundTilesElement );
				GraphicsManager::getInstance()->addDrawable(tile,0);
			}

			//Objects
			TiXmlElement *objectsElement = levelElement->FirstChildElement( "Objects" );
			if( objectsElement )
			{
				TiXmlElement *objectElement = objectsElement->FirstChildElement();
				while( objectElement )
				{
					Entity *objectEntity = entityFcty->createEntity(objectElement->ValueStr());
					if( objectEntity )
					{
						objectEntity->initializeEntity( objectElement );
						entityMgr->registerEntity(objectEntity);
					}
					objectElement = objectElement->NextSiblingElement();
				}
			}
		}

		{
			const int horizontalBlocks = m_levelSize.x / SCREENWIDTH;
			const int verticalBlocks = m_levelSize.y / SCREENHEIGHT;

			for(int h = 0; h < horizontalBlocks; h++)
			{
				for(int v = 0; v < verticalBlocks; v++)
				{
					sf::Vector2f worldPosition(h * SCREENWIDTH * UNRATIO, v * SCREENHEIGHT * UNRATIO * -1.f );
					Entity* entity = new ZoneEntity;
					entity->setPosition(worldPosition);
					entity->initializeEntity();
					entityMgr->registerEntity(entity);
				}
			}
		}

		entityMgr->postLoad();

		TRI_LOG_STR("Load complete.");

		m_sceneLoaded = true;

		Game::getInstance()->resumeFrameTimer();
	}

	void SceneManager::unloadScene(void)
	{
		if(m_backgroundMusic)
		{
			delete m_backgroundMusic;
			m_backgroundMusic = nullptr;
		}

		for(std::vector<Tile*>::iterator iter = m_tileStack.begin(); iter != m_tileStack.end(); iter++)
		{
			delete (*iter);
		}

		m_tileStack.clear();

		m_playerEntity = nullptr;
		m_starEntity = nullptr;

		m_sceneLoaded = false;
	}

	void SceneManager::setUpScene( void )
	{
		TiXmlDocument document("Scenes.xml");
		if(document.LoadFile())
		{
			TiXmlElement* scenesElement = document.FirstChildElement("Scenes");
			if(scenesElement)
			{
				scenesElement->QueryUnsignedAttribute("Continue", &m_sceneNum);

				TiXmlElement* sceneElement = scenesElement->FirstChildElement("Scene");
				while(sceneElement)
				{
					m_sceneFileNameStack.push_back(sceneElement->GetText());
					sceneElement = sceneElement->NextSiblingElement();
				}
			}
		}
	}

	void SceneManager::saveProgress( void )
	{
		TiXmlDocument document("Scenes.xml");
		if(document.LoadFile())
		{
			TiXmlElement* scenesElement = document.FirstChildElement("Scenes");
			if(scenesElement)
			{
				scenesElement->SetAttribute("Continue", m_sceneNum);
			}

			document.SaveFile();
		}
	}
}

