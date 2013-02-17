#ifndef ThrowEntity_h__
#define ThrowEntity_h__

#include <SFML/Graphics.hpp>
#include "Entity.hpp"
#include "../Physics/BodyController.hpp"
#include "../Helper/ScreenTranslator.hpp"

class ThrowEntity : public sb::Entity
{
	DEFINE_ENTITY( ThrowEntity, sb::Entity, 'THRW')

private:
	virtual void update(sf::Time deltaTime);
	virtual void initializeEntity( const TiXmlElement *propertyElement = NULL );

	sb::BodyController	m_throwBody;
	sb::ScreenTranslator m_throwTranslator;
	sf::Sprite			m_throwSprite;
};

#endif // ThrowEntity_h__
