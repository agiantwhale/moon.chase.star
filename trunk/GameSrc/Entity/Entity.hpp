#ifndef ENTITY_H
#define ENTITY_H

#include <string>
#include <boost\any.hpp>
#include <TinyXML\tinyxml.h>
#include <SFML\Graphics\Transformable.hpp>

#include "../Base/Globals.hpp"
#include "../Base/Vec2D.hpp"
#include "../Interface/IEventListener.hpp"
#include "../System/EntityManager.hpp"

using namespace std;

class EntityManager;
class EntityList;

class Entity : public sf::Transformable, public EventListener
{
public:
    virtual void Update(float deltaTime);
    virtual void PostLoad(void);
    virtual bool handleEvent(const EventData& theevent);
    virtual int GetEntityType( void ) const
    {
        return 'BASE';
    }

    void Release( void )
    {
        _released = true;
    }

	void Register(void)
	{
		EntityManager::getInstance()->RegisterEntity(this);
	}

    bool IsReleased( void ) const { return _released; }

    virtual void Initialize( const TiXmlElement *propertyElement = nullptr );
    bool IsInitialized( void ) const { return _initialized; }

	bool IsActive() const { return _active; }
	void SetActive(bool val) { _active = val; }

protected:
    Entity();
    virtual ~Entity();

    bool _initialized;
    bool _released;
	bool _active;		//Inactive entities are not updated.

    friend class EntityManager;
    friend class EntityList;
};

#define DEFINE_ENTITY( ThisName, BaseName, EntType )\
	public:\
	enum\
	{\
		kEntity_##ThisName = EntType\
	};\
	typedef BaseName BaseClass;\
	typedef ThisName ThisClass;\
	friend class EntityBuilder<ThisName>;\
	virtual int GetEntityType( void ) const { return EntType; }\
	protected:\
	ThisName();\
	virtual ~ThisName( void );

#endif
