#ifndef BlockEntity_h__
#define BlockEntity_h__

#include <SFML/Graphics/Sprite.hpp>

#include "Entity.hpp"
#include "../Physics/BodyController.hpp"
#include "../Helper/ScreenPositioner.h"

namespace sf
{
	class Sprite;
}

class BlockEntity : public sb::Entity
{
	DEFINE_ENTITY( BlockEntity, sb::Entity, 'BLCK')

private:
	virtual void initializeEntity( const TiXmlElement *propertyElement = NULL );
	virtual void ProcessContact(const b2Contact* contact, const b2Fixture* contactFixture );
	virtual bool handleEvent(const EventData& theevent);
	virtual void simulate(void);

	bool m_shouldFall;

	sf::Sprite						m_blockSprite;
	sb::BodyController				m_bodyController;
	sb::ScreenPositioner			m_screenPositioner;
};

#endif // BlockEntity_h__
