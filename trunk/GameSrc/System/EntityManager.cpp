#include <CxxTL/tri_logger.hpp>
#include "../App/Game.hpp"
#include "../System/EntityManager.hpp"
#include "../Entity/Entity.hpp"

namespace sb
{
	EntityList::EntityList() :  std::list<Entity*>()
	{
		clear();
	}

	void EntityList::releaseAll(void)
	{
		for( iterator iter = begin(); iter != end(); iter++ )
		{
			Entity *entity = (*iter);
			delete entity;
		}

		clear();
	}

	void EntityList::releaseAllExceptHull(void)
	{
		for( iterator iter = begin(); iter != end(); )
		{
			Entity *entity = (*iter);
			if( entity->getEntityType() != 'HULL' )
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

	void EntityList::delayedRelease( void )
	{
		for( iterator iter = begin(); iter != end(); )
		{
			Entity *entity = (*iter);
			if( entity->isReleased() )
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

	void EntityList::update(sf::Time deltaTime )
	{
		for( iterator iter = begin(); iter != end(); iter++ )
		{
			Entity *entity = (*iter);

			if(entity->isActive())
			{
				entity->update( deltaTime );
			}
		}
	}

	void EntityList::postLoad(void)
	{
		for( iterator iter = begin(); iter != end(); iter++ )
		{
			Entity *entity = (*iter);
			entity->postLoad();
		}
	}

	SINGLETON_CONSTRUCTOR( EntityManager ),
		EventListener("EntityManager"),
		m_entityList()
	{
		addEventListenType(Event_Unload);
		addEventListenType(Event_RestartLevel);
	}

	SINGLETON_DESTRUCTOR( EntityManager )
	{
		removeEventListenType(Event_Unload);
		removeEventListenType(Event_RestartLevel);
		releaseAll();
	}

	bool EntityManager::handleEvent( const EventData& newevent )
	{
		if( newevent.getEventType() == Event_Unload )
		{
			releaseAll();
			return false;
		}

		if( newevent.getEventType() == Event_RestartLevel )
		{
			releaseAllExceptHull();
			return false;
		}

		return false;
	}

	void EntityManager::update(sf::Time deltaTime)
	{
		m_entityList.update(deltaTime);
		m_entityList.delayedRelease();
	}

	void EntityManager::postLoad(void)
	{
		m_entityList.postLoad();
	}

	void EntityManager::releaseAll(void)
	{
		m_entityList.releaseAll();
	}

	void EntityManager::releaseAllExceptHull(void)
	{
		m_entityList.releaseAllExceptHull();
	}

	void EntityManager::registerEntity( Entity* entity )
	{
		if(!entity->isInitialized())
		{
			entity->initializeEntity();
		}

		m_entityList.push_back(entity);
	}

	SINGLETON_CONSTRUCTOR( EntityFactory )
	{
		m_builderMap.clear();
	}

	SINGLETON_DESTRUCTOR( EntityFactory )
	{
	}

	Entity *EntityFactory::createEntity( const std::string& entityClassName )
	{
		Entity *return_ptr = NULL;
		BuilderMap::iterator iter = m_builderMap.find( entityClassName );
		if( iter != m_builderMap.end() )
		{
			return_ptr = iter->second->createEntity();
		}
		else
		{
			TRI_LOG_STR("Entity class unregistered.");
			TRI_LOG(entityClassName);
		}

		return return_ptr;
	}

	void EntityFactory::registerBuilder( EntityBuilder* builder )
	{
		if( m_builderMap.find( builder->getEntityClassName() ) == m_builderMap.end() )
		{
			m_builderMap.insert( std::make_pair( builder->getEntityClassName(), builder ) );
		}
		else
		{
			TRI_LOG_STR("Entity class is already registered.");
			TRI_LOG(builder->getEntityClassName());
		}
	}

	void EntityFactory::deregisterBuilder( EntityBuilder* builder )
	{
		BuilderMap::iterator iter = m_builderMap.find( builder->getEntityClassName() );
		if( iter != m_builderMap.end() )
		{
			m_builderMap.erase( iter );
		}
	}

}