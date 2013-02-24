#include <string>
#include <CxxTL\tri_logger.hpp>
#include <TinyXML\tinyxml.h>

#include "SceneManager.hpp"
#include "../Physics/PhysicsManager.hpp"
#include "GraphicsManager.hpp"
#include "RenderLayer.hpp"

#include "../App/Game.hpp"
#include "../Base/Globals.hpp"
#include "../Entity/EntityManager.hpp"
#include "../Entity/Entity.hpp"
#include "../Tile/Tile.hpp"
#include "../Tile/BackgroundTile.hpp"
#include "../Entity/ZoneEntity.hpp"
#include "../Entity/PlayerEntity.hpp"
#include "../Entity/HullEntity.hpp"
#include "../Helper/Conversion.hpp"
#include "../Event/ContactEventData.hpp"
#include "../Entity/StarEntity.hpp"

namespace sb
{
	SINGLETON_CONSTRUCTOR(SceneManager),
						  EventListener(),
						  m_sceneName(),
						  m_levelSize(),
						  m_gameLost(false),
						  m_gameWon(false),
						  m_sceneLoaded(false),
						  m_sceneNum(0),
						  m_backgroundMusic(nullptr),
						  m_playerEntity(nullptr),
						  m_starEntity(nullptr),
						  m_zoneBody(nullptr)
	{
		addEventListenType(Event_RestartLevel);
		addEventListenType(Event_GameWon);
		addEventListenType(Event_GameLost);
		addEventListenType(Event_Unload);
		addEventListenType(Event_Simulate);
		addEventListenType(Event_EndContact);
	}

	SINGLETON_DESTRUCTOR(SceneManager)
	{
		removeEventListenType(Event_RestartLevel);
		removeEventListenType(Event_GameWon);
		removeEventListenType(Event_GameLost);
		removeEventListenType(Event_Unload);
		removeEventListenType(Event_Simulate);
		removeEventListenType(Event_EndContact);
	}

	bool SceneManager::handleEvent( const EventData& newevent )
	{
		if( newevent.getEventType() == Event_RestartLevel )
		{
			m_sceneLoaded = false;
		}

		if( newevent.getEventType() == Event_Unload )
		{
			unloadScene();
		}

		if(newevent.getEventType() == Event_EndContact)
		{
			if(!m_sceneLoaded) return false;;

			const ContactEventData& contactData = static_cast<const ContactEventData&>(newevent);
			const b2Contact* contactInfo = contactData.getContact();

			const b2Fixture* target = nullptr;

			bool returnValue = false;

			if(contactInfo->GetFixtureA()->GetBody() == m_zoneBody)
			{
				target = contactInfo->GetFixtureB();
				returnValue = true;
			}
			else if(contactInfo->GetFixtureB()->GetBody() == m_zoneBody)
			{
				target = contactInfo->GetFixtureA();
				returnValue = true;
			}

			if(returnValue)
			{
				Entity* targetEntity = getOwnerEntity(target);

				if(targetEntity)
				{
					PlayerEntity* playerEntity = entity_cast<PlayerEntity>(targetEntity);

					if( !m_gameWon && !m_gameLost && playerEntity && m_playerEntity->getPlayerState() != PlayerEntity::kPlayer_Teleport )
					{
						//m_shouldKillMoon = true;
						playerEntity->kill();
					}
					else if(targetEntity->getEntityType() != StarEntity::getEntityIdentifier() && targetEntity->getEntityType() != PlayerEntity::getEntityIdentifier() && targetEntity->getEntityType() != HullEntity::getEntityIdentifier())
					{
						targetEntity->releaseEntity();
					}
				}
			}
		}

		if(newevent.getEventType() == Event_Simulate)
		{
			m_zoneBody->SetTransform(ToVector(GraphicsManager::getInstance()->getRenderLayer(2)->getCamera().getPosition()),0.f);
			m_zoneBody->SetAwake(true);
		}

		if(newevent.getEventType() == Event_GameLost)
		{
			m_gameLost = true;
		}

		if(newevent.getEventType() == Event_GameWon)
		{
			m_gameWon = true;
		}

		return false;
	}

	void SceneManager::restartScene(void)
	{
		m_gameLost = false;
		m_gameWon = false;

		m_playerEntity = nullptr;
		m_starEntity = nullptr;

		TRI_LOG_STR("Restarting scene.");

		TiXmlDocument sceneDocument;

		if( !sceneDocument.LoadFile(m_sceneInfoStack[m_sceneNum].fileName) )
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

		{
			m_zoneBody->SetTransform(ToVector(GraphicsManager::getInstance()->getRenderLayer(2)->getCamera().getPosition()),0.f);
			m_zoneBody->SetAwake(true);
		}

		entityMgr->postLoad();

		TRI_LOG_STR("Load complete.");

		m_sceneLoaded = true;
	}

	void SceneManager::loadScene( unsigned int sceneNum )
	{
		m_gameLost = false;
		m_gameWon = false;

		if(sceneNum >= m_sceneInfoStack.size())
		{
			sceneNum = 0;
		}
		std::string sceneFileName = m_sceneInfoStack.at(sceneNum).fileName;

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

			levelElement->QueryStringAttribute("SceneName",&m_sceneName);

			std::string musicFileName;
			levelElement->QueryStringAttribute("BackgroundMusic", &musicFileName);
			if(!musicFileName.empty())
			{
				if(m_backgroundMusic)
				{
					clearMusic();
				}

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
				GraphicsManager::getInstance()->addDrawable(*tile,1);
			}

			//Tiles
			TiXmlElement *backgroundTilesElement = levelElement->FirstChildElement("Backgrounds");
			if(backgroundTilesElement)
			{
				BackgroundTile* tile = new BackgroundTile( backgroundTilesElement );
				GraphicsManager::getInstance()->addDrawable(*tile,0);
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

		{
			b2BodyDef bodyDefinition;
			bodyDefinition.userData = nullptr;
			bodyDefinition.position = ToVector(GraphicsManager::getInstance()->getRenderLayer(2)->getCamera().getPosition());
			bodyDefinition.angle = 0.0f;
			bodyDefinition.fixedRotation = true;
			bodyDefinition.type = b2_dynamicBody;
			bodyDefinition.gravityScale = 0.f;

			m_zoneBody = PhysicsManager::getInstance()->getPhysicsWorld()->CreateBody(&bodyDefinition);

			b2PolygonShape boxShape;
			boxShape.SetAsBox( 0.5f * SCREENWIDTH * UNRATIO, 0.5f * SCREENHEIGHT * UNRATIO );

			b2FixtureDef fixtureDef;
			fixtureDef.isSensor = true;
			fixtureDef.shape = &boxShape;

			m_zoneBody->CreateFixture(&fixtureDef);
		}

		entityMgr->postLoad();

		TRI_LOG_STR("Load complete.");

		m_sceneLoaded = true;

		Game::getInstance()->resumeFrameTimer();
	}

	void SceneManager::unloadScene(void)
	{
		for(std::vector<Tile*>::iterator iter = m_tileStack.begin(); iter != m_tileStack.end(); iter++)
		{
			GraphicsManager::getInstance()->removeDrawable(*(*iter), (*iter)->getLayer());
			delete (*iter);
		}

		PhysicsManager::getInstance()->getPhysicsWorld()->DestroyBody(m_zoneBody);
		m_zoneBody = nullptr;

		m_tileStack.clear();

		m_playerEntity = nullptr;
		m_starEntity = nullptr;

		m_sceneLoaded = false;
	}

	void SceneManager::setUpScene( const TiXmlElement* element )
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
					SceneInfo info;
					info.fileName = sceneElement->FirstChildElement("File")->GetText();
					info.helperText = sceneElement->FirstChildElement("Help")->GetText();

					m_sceneInfoStack.push_back(info);
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

	const std::string& SceneManager::getHelperText( unsigned int sceneNum )
	{
		if(sceneNum >= m_sceneInfoStack.size())
		{
			sceneNum = 0;
		}

		return m_sceneInfoStack.at(sceneNum).helperText;
	}

}

