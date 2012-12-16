#ifndef ENTITYMANAGER_H
#define ENTITYMANAGER_H

#include <list>
#include <string>
#include <unordered_map>
#include "../Base/Singleton.hpp"

using namespace std;

class Entity;
class IEntityBuilder;

class EntityList : private std::unordered_multimap<int, Entity*>
{
public:
	EntityList();

	void Add( Entity* entity );
	void DelayedRelease( void );
	void ReleaseAll(void);

	void Update( float deltaTime );
	void PostLoad(void);
};

class EntityManager : public Singleton<EntityManager>
{
	DEFINE_SINGLETON( EntityManager )

public:
	void Update(float deltaTime);
	void PostLoad(void);
	void ReleaseAll(void);
	void RegisterEntity( Entity* entity );

	/*
	void LoadScene( const string& sceneFileName );
	void UnloadScene( void );
	inline void ReloadScene(void)
	{
	    UnloadScene();
	    LoadScene(_sceneFileName);
	}

	inline void SetNextScene( const std::string nextSceneFileName )
	{
	    _nextSceneFileName = nextSceneFileName;
	}
	*/

private:
    EntityList _entityList;
    //std::string _sceneFileName;
    //std::string _nextSceneFileName;
};

class EntityFactory : public Singleton<EntityFactory>
{
	DEFINE_SINGLETON( EntityFactory )

public:
	Entity *CreateEntity( const string& entityClassName );

	void RegisterBuilder( IEntityBuilder* builder );
	void DeregisterBuilder( IEntityBuilder* builder );

private:
	typedef std::unordered_map<string,IEntityBuilder*> BuilderMap;

	BuilderMap _builderMap;
};

class IEntityBuilder
{
public:
	virtual std::string GetEntityClassName( void ) = 0;
	virtual Entity		*CreateEntity( void ) = 0;
};

template<typename type>
class EntityBuilder : public IEntityBuilder
{
public:
	EntityBuilder( const std::string& entityClassName ) :	IEntityBuilder(),
                                                            _entityClassName( entityClassName )

	{
		EntityFactory::GetInstance()->RegisterBuilder( this );
	}

	virtual ~EntityBuilder()
	{
		EntityFactory::GetInstance()->DeregisterBuilder( this );
	}

	virtual std::string GetEntityClassName( void ) {
		return _entityClassName;
	}

	virtual Entity *CreateEntity( void ) {
		return new type;
	}

private:
	std::string _entityClassName;
};

#define REGISTER_ENTITY( ThisName,  EntName )\
	EntityBuilder<ThisName> EntReg_##ThisName( EntName );


#endif
