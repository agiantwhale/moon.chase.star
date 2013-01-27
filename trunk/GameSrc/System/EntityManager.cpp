#include <CxxTL/tri_logger.hpp>
#include "../App/Game.hpp"
#include "../System/EntityManager.hpp"
#include "../Entity/Entity.hpp"

EntityList::EntityList() :  std::map<int,Entity*>()
{
    clear();
}

void EntityList::ReleaseAll(void)
{
    for( iterator iter = begin(); iter != end(); iter++ )
    {
        Entity *entity = (iter->second);
        delete entity;
    }

    clear();
}

void EntityList::ReleaseAllExceptHull(void)
{
    for( iterator iter = begin(); iter != end(); )
    {
        Entity *entity = (iter->second);
        if( entity->GetEntityType() != 'HULL' )
        {
            delete entity;
            iter = erase(iter);
        }
        else
        {
            iter++;
        }
    }
}

void EntityList::Add( Entity *entity )
{
    if( entity == NULL ) return;

    if(!entity->IsInitialized())
    {
        entity->Initialize();
    }

    insert( make_pair( size(), entity ) );
}

void EntityList::DelayedRelease( void )
{
    for( iterator iter = begin(); iter != end(); )
    {
        Entity *entity = (iter->second);
        if( entity->IsReleased() )
        {
            iter = erase( iter );
            delete entity;
        }
        else
        {
            iter++;
        }
    }
}

void EntityList::Update(float deltaTime )
{
    for( iterator iter = begin(); iter != end(); iter++ )
    {
        Entity *entity = (iter->second);

		if(entity->IsActive())
		{
			entity->Update( deltaTime );
		}
    }
}

void EntityList::PostLoad(void)
{
    for( iterator iter = begin(); iter != end(); iter++ )
    {
        Entity *entity = (iter->second);
        entity->PostLoad();
    }
}

SINGLETON_CONSTRUCTOR( EntityManager ),
                       IEventListener("EntityManager"),
                       _entityList()
{
	AddEventListenType(Event_Unload);
	AddEventListenType(Event_RestartLevel);
}

SINGLETON_DESTRUCTOR( EntityManager )
{
	RemoveEventListenType(Event_Unload);
	RemoveEventListenType(Event_RestartLevel);
    ReleaseAll();
}

bool EntityManager::HandleEvent( const EventData& newevent )
{
    if( newevent.GetEventType() == Event_Unload )
    {
        ReleaseAll();
        return false;
    }

    if( newevent.GetEventType() == Event_RestartLevel )
    {
        ReleaseAllExceptHull();
        return false;
    }

    return false;
}

void EntityManager::Update(float deltaTime)
{
    _entityList.Update(deltaTime);
}

void EntityManager::PostLoad(void)
{
    _entityList.PostLoad();
}

void EntityManager::ReleaseAll(void)
{
    _entityList.ReleaseAll();
}

void EntityManager::ReleaseAllExceptHull(void)
{
    _entityList.ReleaseAllExceptHull();
}

void EntityManager::RegisterEntity( Entity* entity )
{
    _entityList.Add(entity);
}

SINGLETON_CONSTRUCTOR( EntityFactory )
{
    _builderMap.clear();
}

SINGLETON_DESTRUCTOR( EntityFactory )
{
}

Entity *EntityFactory::CreateEntity( const string& entityClassName )
{
    Entity *return_ptr = NULL;
    BuilderMap::iterator iter = _builderMap.find( entityClassName );
    if( iter != _builderMap.end() )
    {
        return_ptr = iter->second->CreateEntity();
    }
    else
    {
		TRI_LOG_STR("Entity class unregistered.");
		TRI_LOG(entityClassName);
	}

    return return_ptr;
}

void EntityFactory::RegisterBuilder( IEntityBuilder* builder )
{
    if( _builderMap.find( builder->GetEntityClassName() ) == _builderMap.end() )
    {
        _builderMap.insert( make_pair( builder->GetEntityClassName(), builder ) );
    }
    else
    {
		TRI_LOG_STR("Entity class is already registered.");
		TRI_LOG(builder->GetEntityClassName());
    }
}

void EntityFactory::DeregisterBuilder( IEntityBuilder* builder )
{
    BuilderMap::iterator iter = _builderMap.find( builder->GetEntityClassName() );
    if( iter != _builderMap.end() )
    {
        _builderMap.erase( iter );
    }
}
