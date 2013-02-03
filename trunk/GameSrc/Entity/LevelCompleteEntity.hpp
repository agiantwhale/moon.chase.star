#ifndef LEVELCOMPLETEENTITY_HPP
#define LEVELCOMPLETEENTITY_HPP

#include "../Entity/Entity.hpp"
#include "../Entity/Component/BodyWrapper.hpp"

class LevelCompleteEntity : public Entity
{
	DEFINE_ENTITY(LevelCompleteEntity,Entity,'LVL')

public:
	virtual void Initialize( const TiXmlElement *propertyElement = NULL );
	virtual void ProcessContact(const b2Contact* contact, const b2Fixture* contactFixture );
	virtual bool HandleEvent( const EventData& theevent );

private:
	BodyWrapper _triggerBody;
};

#endif