#ifndef ENTITY_H
#define ENTITY_H

#include <string>
#include <boost\any.hpp>
#include <TinyXML\tinyxml.h>

#include "../Base/Globals.hpp"
#include "../Base/Vec2D.hpp"
#include "../Interface/ITransform.hpp"
#include "../Interface/IEventListner.hpp"
#include "../System/EntityManager.hpp"

using namespace std;

class EntityManager;
class EntityList;

class Entity : public ITransform, public IEventListener
{
public:
	virtual void Update(float deltaTime);
	virtual void PostLoad(void);
	virtual bool HandleEvent(const EventData& theevent);
	virtual int GetEntityType( void ) const { return 'BASE'; }
	static int GetEntityClassType( void ) { return 'BASE'; }
	virtual const std::string& GetEntityName(void) const {return "Entity";}
	virtual const std::string& GetEventListenerName() const {return GetEntityName();}

	inline void Release( void ) { _released = true; }
	inline const bool IsReleased( void ) const { return _released; }

	virtual void Initialize( const TiXmlElement *propertyElement = nullptr );
	inline const bool IsInitialized( void ) const { return _initialized; }

protected:
    Entity();
	virtual ~Entity();

    bool _initialized;
	bool _released;

	friend class EntityManager;
	friend class EntityList;
};

template< class T >
T* entity_cast( Entity *p )
{
	if( p->GetEntityType() == T::GetEntityClassType() )
	{
		return static_cast<T*>(p);
	}

	return NULL;
}

#define DEFINE_ENTITY( ThisName, BaseName, EntType, EntName )\
	public:\
	enum\
	{\
		kEntity_##ThisName = EntType\
	};\
	typedef BaseName BaseClass;\
	typedef ThisName ThisClass;\
	friend class EntityBuilder<ThisName>;\
	virtual int GetEntityType( void ) const { return EntType; }\
	static int GetEntityClassType( void ) { return EntType; }\
	virtual const std::string& GetEntityName(void) const {return EntName;}\
	protected:\
	ThisName();\
	virtual ~ThisName( void );

#endif
