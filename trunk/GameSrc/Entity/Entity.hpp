#ifndef Entity_h__
#define Entity_h__

#include <TinyXML\tinyxml.h>
#include <SFML\Graphics\Transformable.hpp>
#include <SFML\System\Time.hpp>

#include "../Base/Globals.hpp"
#include "../Event/EventListener.hpp"
#include "EntityManager.hpp"

namespace sb
{
	class EntityManager;
	class EntityList;

	class Entity : public sf::Transformable, public EventListener
	{
	public:
		virtual void update(sf::Time deltaTime);
		virtual void postLoad(void);
		virtual bool handleEvent(const EventData& theevent);
		virtual int getEntityType( void ) const
		{
			return 'BASE';
		}
		static int getEntityIdentifier(void)
		{
			return 'BASE';
		}

		void releaseEntity( void )
		{
			m_released = true;
		}

		void registerEntity(void)
		{
			EntityManager::getInstance()->registerEntity(this);
		}

		bool isReleased( void ) const { return m_released; }

		virtual void initializeEntity( const TiXmlElement *propertyElement = nullptr );
		bool isInitialized( void ) const { return m_initialized; }

		bool isActive() const { return m_active; }
		void setActive(bool val) { m_active = val; }

	protected:
		Entity();
		virtual ~Entity();

		bool m_initialized;
		bool m_released;
		bool m_active;		//Inactive entities are not updated.

		friend class EntityManager;
		friend class EntityList;
	};

	template< class T >
	T* entity_cast( Entity* p )
	{
		if ( p && p->getEntityType() == T::getEntityIdentifier() )
		{
			return static_cast<T*>(p);
		}

		return nullptr;	
	}
}

#define DEFINE_ENTITY( ThisName, BaseName, EntType )						\
	public:																	\
	typedef BaseName BaseClass;												\
	typedef ThisName ThisClass;												\
	friend class sb::EntityClassBuilder<ThisName>;							\
	virtual int getEntityType( void ) const { return EntType; }				\
	static	int getEntityIdentifier(void) { return EntType; }				\
	protected:																\
	ThisName();																\
	virtual ~ThisName( void );												\

#endif // Entity_h__
