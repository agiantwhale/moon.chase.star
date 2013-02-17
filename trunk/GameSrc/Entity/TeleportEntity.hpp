#ifndef TeleportEntity_h__
#define TeleportEntity_h__

#include <SFML/Graphics.hpp>
#include "Entity.hpp"
#include "../Physics/BodyController.hpp"
#include "../Helper/ScreenTranslator.hpp"

class TeleportEntity : public sb::Entity
{
	DEFINE_ENTITY( TeleportEntity, sb::Entity, 'TLPT')

public:
	const sf::Vector2f& getExitPosition() {return m_exitTransform.getPosition();}

private:
	virtual void update(sf::Time deltaTime);
	virtual void initializeEntity( const TiXmlElement *propertyElement = NULL );

	sf::Transformable		m_entranceTransform;
	sf::Transformable		m_exitTransform;
	sb::ScreenTranslator	m_entranceTranslator;
	sb::ScreenTranslator	m_exitTranslator;

	sb::BodyController		m_triggerBody;
	sf::Sprite	m_enterSprite;
	sf::Sprite	m_exitSprite;
};

#endif // TeleportEntity_h__
