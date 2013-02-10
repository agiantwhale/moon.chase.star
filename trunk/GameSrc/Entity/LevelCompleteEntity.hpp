#ifndef LEVELCOMPLETEENTITY_HPP
#define LEVELCOMPLETEENTITY_HPP

#include "../Base/Math.hpp"
#include "../Entity/Entity.hpp"
#include "../Entity/Component/BodyWrapper.hpp"

class PlayerEntity;

class LevelCompleteEntity : public Entity
{
	DEFINE_ENTITY(LevelCompleteEntity,Entity,'LVL')

public:
	virtual void Initialize( const TiXmlElement *propertyElement = NULL );
	virtual void ProcessContact(const b2Contact* contact, const b2Fixture* contactFixture );
	virtual bool HandleEvent( const EventData& theevent );

private:
	typedef std::vector<Vec2D> VectorStack;
	VectorStack	_endFollowRoute;

	BodyWrapper _triggerBody;
	bool		_acceptArrival;
};

#endif