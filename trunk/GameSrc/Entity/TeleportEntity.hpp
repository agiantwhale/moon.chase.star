#ifndef TeleportEntity_h__
#define TeleportEntity_h__

#include <SFML/Graphics.hpp>
#include "Entity.hpp"
#include "../Physics/BodyController.hpp"
#include "../Helper/ScreenTranslator.hpp"

class TeleportEntity;

class TeleportExitEntity : public sb::Entity
{
	DEFINE_ENTITY( TeleportExitEntity, sb::Entity, 'TLXT')

private:
	virtual void update(sf::Time deltaTime);
	virtual void initializeEntity( const TiXmlElement *propertyElement = NULL );

	sf::Transformable		m_exitTransform;
	sb::ScreenTranslator	m_exitTranslator;
	sb::BodyController		m_triggerBody;
	sf::Sprite	m_exitSprite;

	friend class TeleportEntity;
};

class TeleportEntity : public sb::Entity
{
	DEFINE_ENTITY( TeleportEntity, sb::Entity, 'TLPT')

public:
	const sf::Vector2f& getExitPosition() {return m_exitEntity->getPosition();}

private:
	virtual void update(sf::Time deltaTime);
	virtual void initializeEntity( const TiXmlElement *propertyElement = NULL );

	TeleportExitEntity*		m_exitEntity;

	sf::Transformable		m_entranceTransform;
	sb::ScreenTranslator	m_entranceTranslator;
	sb::BodyController		m_triggerBody;
	sf::Sprite	m_enterSprite;
};

#endif // TeleportEntity_h__
