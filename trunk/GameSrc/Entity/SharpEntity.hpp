#ifndef SharpEntity_h__
#define SharpEntity_h__

#include <SFML/Graphics/Sprite.hpp>
#include "../Entity/Entity.hpp"
#include "../Helper/ScreenTranslator.hpp"
#include "../Physics/BodyController.hpp"

class SharpEntity : public sb::Entity
{
	DEFINE_ENTITY( SharpEntity, sb::Entity, 'SHRP')

private:
	virtual void initializeEntity( const TiXmlElement *propertyElement = NULL );

	virtual void update( sf::Time deltaTime );

	sf::Sprite				m_sharpSprite;
	sb::ScreenTranslator	m_sharpTranslator;
	sb::BodyController		m_sharpBody;
};

#endif // SharpEntity_h__
