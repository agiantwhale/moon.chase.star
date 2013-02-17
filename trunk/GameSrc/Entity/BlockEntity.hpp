#ifndef BlockEntity_h__
#define BlockEntity_h__

#include <SFML/Graphics/Sprite.hpp>

#include "Entity.hpp"
#include "../Event/EventData.hpp"
#include "../Physics/BodyController.hpp"
#include "../Helper/ScreenTranslator.hpp"

namespace sf
{
	class Sprite;
}

class BlockEntity : public sb::Entity
{
	DEFINE_ENTITY( BlockEntity, sb::Entity, 'BLCK')

private:
	virtual void initializeEntity( const TiXmlElement *propertyElement = NULL );
	virtual void processContact(const b2Contact* contact, const b2Fixture* contactFixture );
	virtual bool handleEvent(const sb::EventData& theevent);
	virtual void simulate(void);

	bool m_shouldFall;

	sf::Sprite						m_blockSprite;
	sb::BodyController				m_bodyController;
	sb::ScreenTranslator			m_screenPositioner;
};

#endif // BlockEntity_h__
