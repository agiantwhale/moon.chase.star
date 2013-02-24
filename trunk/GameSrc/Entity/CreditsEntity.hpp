#ifndef CreditsEntity_h__
#define CreditsEntity_h__

#include <SFML/System/Time.hpp>
#include <SFML/Graphics/Text.hpp>
#include "Entity.hpp"
#include "../Physics/BodyController.hpp"
#include "../Base/Math.hpp"

class CreditsEntity : public sb::Entity
{
	DEFINE_ENTITY(CreditsEntity,sb::Entity,'CRDT')

private:
	virtual void initializeEntity( const TiXmlElement *propertyElement = NULL );
	virtual bool handleEvent( const sb::EventData& theevent );
	virtual void update( sf::Time deltaTime );

	int			 m_textSize;
	sf::Vector2f m_textPos;
	sf::Text	 m_text;
	sb::BodyController	m_triggerBody;
	bool m_triggered;

	sf::Time m_variationTime;
	sf::Time m_totalTime;
};

#endif // CreditsEntity_h__
