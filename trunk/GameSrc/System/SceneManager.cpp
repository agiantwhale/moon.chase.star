#include <glog\logging.h>
#include <TinyXML\tinyxml.h>
#include "../Base/Globals.hpp"
#include "../System/SceneManager.hpp"
#include "../System/EntityManager.hpp"
#include "../System/ResourceManager.hpp"
#include "../Entity/Entity.hpp"
#include "../Tile/Tile.hpp"

SINGLETON_CONSTRUCTOR(SceneManager), IEventListener("SceneManager"), _sceneLoaded(false), _sceneFileName()
{
}

SINGLETON_DESTRUCTOR(SceneManager)
{
}

bool SceneManager::HandleEvent( const EventData& newevent )
{
	if( newevent.GetEventType() == Event_Unload )
	{
		UnloadScene();
		return false;
	}

	if( newevent.GetEventType() == Event_RestartLevel )
	{
		RestartScene( _sceneFileName );
		return false;
	}

	return false;
}

void SceneManager::RestartScene(const std::string& sceneFileName)
{
	LOG(INFO) << "Restarting scene " << sceneFileName << "...";

	TiXmlDocument sceneDocument;

	if( !sceneDocument.LoadFile(sceneFileName) )
	{
		LOG(WARNING) << "Unable to load scene " << sceneFileName << "!";
	    return;
	}
	else
	{
	    _sceneFileName = sceneFileName;
	}

	EntityFactory* entityFcty = EntityFactory::GetInstance();
	EntityManager* entityMgr = EntityManager::GetInstance();

	TiXmlElement *levelElement = sceneDocument.FirstChildElement( "level" );
	if( !levelElement )
	{
	    LOG(WARNING) << "Messed up scene structure, unable to find \"level\" element.";
	}
	else
	{
		TiXmlElement *objectsElement = levelElement->FirstChildElement( "Objects" );
		if( objectsElement )
		{
			TiXmlElement *objectElement = objectsElement->FirstChildElement();
			while( objectElement )
			{
				Entity *objectEntity = entityFcty->CreateEntity(objectElement->ValueStr());
				if( objectEntity )
				{
				    objectEntity->Initialize( objectElement );
					entityMgr->RegisterEntity(objectEntity);
				}
				objectElement = objectElement->NextSiblingElement();
			}
		}
	}

	entityMgr->PostLoad();

	LOG(INFO) << "Load Complete.";

	_sceneLoaded = true;
}

void SceneManager::LoadScene(const std::string& sceneFileName)
{
	LOG(INFO) << "Loading scene " << sceneFileName << "...";

	TiXmlDocument sceneDocument;

	if( !sceneDocument.LoadFile(sceneFileName) )
	{
		LOG(WARNING) << "Unable to load scene " << sceneFileName << "!";
	    return;
	}
	else
	{
	    _sceneFileName = sceneFileName;
	}

	EntityFactory* entityFcty = EntityFactory::GetInstance();
	EntityManager* entityMgr = EntityManager::GetInstance();

	TiXmlElement *levelElement = sceneDocument.FirstChildElement( "level" );
	if( !levelElement )
	{
	    LOG(WARNING) << "Messed up scene structure, unable to find \"level\" element.";
	}
	else
	{
		/*
		//Initialize all this later, after loading all crap from the scene file.

	    int levelWidth = 0.0f, levelHeight = 0.0f;
	    levelElement->Attribute("width",&levelWidth);
	    levelElement->Attribute("height",&levelHeight);
        //TheLightMgr.Create(AABB(Vec2f(0, 0), Vec2f(levelWidth,levelHeight)),&TheApp,"Resource/Light/lightFin.png","Resource/Shaders/lightAttenuationShader.frag");

        int camSlice = 0;
        levelElement->Attribute("camSlice",&camSlice);
        const int horiSlice = levelWidth / SCREENWIDTH;
        //cameraMgr->SetPosition(sf::Vector2f(SCREENWIDTH*UNRATIO*(camSlice%horiSlice),-SCREENHEIGHT*UNRATIO*static_cast<int>(camSlice/horiSlice)));
		*/

	    //Collision
	    TiXmlElement *collisionElement = levelElement->FirstChildElement("Collision");
	    if(collisionElement)
	    {
	        TiXmlElement *hullElement = collisionElement->FirstChildElement("rect");
	        while(hullElement)
	        {
	            Entity *hullEntity = entityFcty->CreateEntity("Hull");
	            if(hullEntity)
	            {
	                hullEntity->Initialize(hullElement);
					entityMgr->RegisterEntity( hullEntity );
	            }
	            hullElement = hullElement->NextSiblingElement();
	        }
	    }

	    //Tiles
	    TiXmlElement *tilesElement = levelElement->FirstChildElement("Tiles");
	    if(tilesElement)
	    {
			Tile* tile = new Tile();
			tile->Initialize( tilesElement );
			tile->RegisterRenderable( 0 );
	    }

		//Objects
		TiXmlElement *objectsElement = levelElement->FirstChildElement( "Objects" );
		if( objectsElement )
		{
			TiXmlElement *objectElement = objectsElement->FirstChildElement();
			while( objectElement )
			{
				Entity *objectEntity = entityFcty->CreateEntity(objectElement->ValueStr());
				if( objectEntity )
				{
				    objectEntity->Initialize( objectElement );
					entityMgr->RegisterEntity(objectEntity);
				}
				objectElement = objectElement->NextSiblingElement();
			}
		}
	}

	entityMgr->PostLoad();

	LOG(INFO) << "Load Complete.";

	_sceneLoaded = true;
}

void SceneManager::UnloadScene(void)
{
	_sceneLoaded = false;
}