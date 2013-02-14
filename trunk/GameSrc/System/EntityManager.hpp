#ifndef EntityManager_h__
#define EntityManager_h__

#include <list>
#include <string>
#include <unordered_map>
#include <SFML/System/Time.hpp>
#include "../Base/Singleton.hpp"
#include "../Event/EventListener.hpp"

namespace sb
{
	class Entity;
	class EntityBuilder;

	class EntityList : public std::list<Entity*>
	{
	public:
		EntityList();

		void delayedRelease( void );
		void releaseAll(void);
		void releaseAllExceptHull(void);

		void update( sf::Time deltaTime );
		void postLoad(void);
	};

	class EntityManager : public Singleton<EntityManager>, private EventListener
	{
		DEFINE_SINGLETON( EntityManager )

	public:
		void update(sf::Time deltaTime);
		void postLoad(void);
		void releaseAll(void);
		void releaseAllExceptHull(void);
		void registerEntity( Entity* entity );

	private:
		virtual bool handleEvent( const EventData& newevent );

	private:
		EntityList m_entityList;
	};

	class EntityFactory : public Singleton<EntityFactory>
	{
		DEFINE_SINGLETON( EntityFactory )

	public:
		Entity *createEntity( const std::string& entityClassName );

		void registerBuilder( EntityBuilder* builder );
		void deregisterBuilder( EntityBuilder* builder );

	private:
		typedef std::unordered_map<std::string,EntityBuilder*> BuilderMap;

		BuilderMap m_builderMap;
	};

	class EntityBuilder
	{
	public:
		virtual std::string getEntityClassName( void ) = 0;
		virtual Entity		*createEntity( void ) = 0;
	};

	template<typename type>
	class EntityClassBuilder : public EntityBuilder
	{
	public:
		EntityClassBuilder( const std::string& entityClassName ) :	EntityBuilder(),
			m_entityClassName( entityClassName )

		{
			EntityFactory::getInstance()->registerBuilder( this );
		}

		virtual ~EntityClassBuilder()
		{
			EntityFactory::getInstance()->deregisterBuilder( this );
		}

		virtual std::string getEntityClassName( void )
		{
			return m_entityClassName;
		}

		virtual Entity *createEntity( void )
		{
			return new type;
		}

	private:
		std::string m_entityClassName;
	};
}

#define REGISTER_ENTITY( ThisName,  EntName )\
	sb::EntityClassBuilder<ThisName> EntReg_##ThisName( EntName );

#endif // EntityManager_h__
