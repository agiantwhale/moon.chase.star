#include <CxxTL\tri_logger.hpp>
#include <TinyXML\tinyxml.h>
#include "../App/Game.hpp"
#include "../Base/Globals.hpp"
#include "../System/SceneManager.hpp"
#include "../System/EntityManager.hpp"
#include "../Entity/Entity.hpp"
#include "../Tile/Tile.hpp"
#include "../Tile/BackgroundTile.hpp"
#include "GraphicsManager.hpp"

SINGLETON_CONSTRUCTOR(SceneManager),
                      IEventListener("SceneManager"),
					  _levelSize(),
                      _sceneLoaded(false),
                      _sceneNum(0)
{
	AddEventListenType(Event_Unload);
}

SINGLETON_DESTRUCTOR(SceneManager)
{
	RemoveEventListenType(Event_Unload);
}

bool SceneManager::HandleEvent( const EventData& newevent )
{
    if( newevent.GetEventType() == Event_Unload )
    {
        UnloadScene();
        return false;
    }

    return false;
}

void SceneManager::RestartScene(void)
{
	TRI_LOG_STR("Restarting scene.");

    TiXmlDocument sceneDocument;

    if( !sceneDocument.LoadFile(_sceneFileNameStack[_sceneNum]) )
    {
        TRI_LOG_STR("Unable to load scene.");
		TRI_LOG(_sceneNum);
        return;
    }

    EntityFactory* entityFcty = EntityFactory::GetInstance();
    EntityManager* entityMgr = EntityManager::GetInstance();

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
                Entity *objectEntity = entityFcty->CreateEntity(objectElement->ValueStr());
                if( objectEntity )
                {
                    objectEntity->Initialize( objectElement );
                    entityMgr->RegisterEntity(objectEntity);
                }
                objectElement = objectElement->NextSiblingElement();
            }
        }

		int camSlice = 0;
		levelElement->Attribute("camSlice",&camSlice);
		const int horiSlice = _levelSize.x / SCREENWIDTH;

		for(int i = 0; i < GraphicsManager::GetInstance()->GetRenderLayerStackSize(); i++ )
		{
			GraphicsManager::GetInstance()->GetRenderLayer(i)->GetCamera().SetPosition(Vec2D(SCREENWIDTH*UNRATIO*(camSlice%horiSlice),-SCREENHEIGHT*UNRATIO*static_cast<int>(camSlice/horiSlice)));
		}
    }

    entityMgr->PostLoad();

    TRI_LOG_STR("Load complete.");

    _sceneLoaded = true;
}

void SceneManager::LoadScene( unsigned int sceneNum )
{
	if(sceneNum >= _sceneFileNameStack.size())
	{
		sceneNum = 0;
	}
	std::string sceneFileName = _sceneFileNameStack.at(sceneNum);

	Game::GetInstance()->PauseFrameTimer();

    TRI_LOG_STR("Loading scene.");

    TiXmlDocument sceneDocument;

    if( !sceneDocument.LoadFile(sceneFileName) )
    {
		TRI_LOG_STR("Unable to load scene.");
		TRI_LOG(_sceneNum);
		return;
    }
	else
	{
		_sceneNum = sceneNum;
	}

    EntityFactory* entityFcty = EntityFactory::GetInstance();
    EntityManager* entityMgr = EntityManager::GetInstance();

    TiXmlElement *levelElement = sceneDocument.FirstChildElement( "level" );
    if( !levelElement )
    {
        TRI_LOG_STR("Wrong scene structure.");
    }
    else
    {
        //Initialize all this later, after loading all crap from the scene file.

        int levelWidth = 0.0f, levelHeight = 0.0f;
        levelElement->Attribute("width",&levelWidth);
        levelElement->Attribute("height",&levelHeight);
		_levelSize.Set(levelWidth,levelHeight);

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
            tile->RegisterRenderable( 1 );
        }

		//Tiles
		TiXmlElement *backgroundTilesElement = levelElement->FirstChildElement("Backgrounds");
		if(backgroundTilesElement)
		{
			BackgroundTile* tile = new BackgroundTile();
			tile->Initialize( backgroundTilesElement );
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

		int camSlice = 0;
		levelElement->Attribute("camSlice",&camSlice);
		const int horiSlice = levelWidth / SCREENWIDTH;

		for(int i = 0; i < GraphicsManager::GetInstance()->GetRenderLayerStackSize(); i++ )
		{
			GraphicsManager::GetInstance()->GetRenderLayer(i)->GetCamera().SetPosition(Vec2D(SCREENWIDTH*UNRATIO*(camSlice%horiSlice),-SCREENHEIGHT*UNRATIO*static_cast<int>(camSlice/horiSlice)));
		}
    }

    entityMgr->PostLoad();

    TRI_LOG_STR("Load complete.");

    _sceneLoaded = true;

	Game::GetInstance()->ResumeFrameTimer();
}

void SceneManager::UnloadScene(void)
{
	for(std::vector<Tile*>::iterator iter = _tileStack.begin(); iter != _tileStack.end(); iter++)
	{
		delete (*iter);
	}

	_tileStack.clear();
    _sceneLoaded = false;
}

void SceneManager::SetUpScene( void )
{
	TiXmlDocument document("Scenes.xml");
	if(document.LoadFile())
	{
		TiXmlElement* scenesElement = document.FirstChildElement("Scenes");
		if(scenesElement)
		{
			scenesElement->QueryFloatAttribute("Continue", _sceneNum);

			TiXmlElement* sceneElement = scenesElement->FirstChildElement("Scene");
			while(sceneElement)
			{
				_sceneFileNameStack.push_back(sceneElement->GetText());
				sceneElement = sceneElement->NextSiblingElement();
			}
		}
	}
}
