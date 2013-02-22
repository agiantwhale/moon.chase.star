#ifndef GravityEntity_h__
#define GravityEntity_h__

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "Entity.hpp"
#include "../Physics/PhysicsManager.hpp"
#include "../Physics/BodyController.hpp"
#include "../Helper/ScreenTranslator.hpp"

class PlayerEntity;

class GravityEntity : public sb::Entity
{
	DEFINE_ENTITY( GravityEntity, sb::Entity, 'GRAV')

public:
	sb::PhysicsManager::GravityDirection getGravityDirection() const { return m_gravityDirection; }

private:
	virtual void update(sf::Time deltaTime);
	virtual void initializeEntity( const TiXmlElement *propertyElement = NULL );
	virtual bool handleEvent(const sb::EventData& theevent);
	void simulate();

	sb::PhysicsManager::GravityDirection m_gravityDirection;
	sb::BodyController	m_gravityBody;
	sb::ScreenTranslator m_gravityTranslator;
	sf::Sprite			m_gravitySprite;
	sf::Sound			m_gravitySound;

	bool m_changeGravity;
};

#endif // GravityEntity_h__
