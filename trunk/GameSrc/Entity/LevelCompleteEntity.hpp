#ifndef LevelCompleteEntity_h__
#define LevelCompleteEntity_h__

#include "Entity.hpp"
#include "../Physics/BodyController.hpp"
#include "../Base/Math.hpp"

class PlayerEntity;

class LevelCompleteEntity : public sb::Entity
{
	DEFINE_ENTITY(LevelCompleteEntity,sb::Entity,'LVLC')

private:
	virtual void initializeEntity( const TiXmlElement *propertyElement = NULL );
	virtual void processContact(const b2Contact* contact, const b2Fixture* contactFixture );
	virtual bool handleEvent( const sb::EventData& theevent );

	typedef std::vector<sf::Vector2f> VectorStack;
	VectorStack	m_endFollowRoute;

	sb::BodyController	m_triggerBody;
	bool		m_acceptArrival;
};

#endif // LevelCompleteEntity_h__
