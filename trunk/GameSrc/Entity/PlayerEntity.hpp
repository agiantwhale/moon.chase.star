#ifndef PlayerEntity_h__
#define PlayerEntity_h__

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <list>
#include "Entity.hpp"
#include "../Physics/BodyController.hpp"
#include "../Helper/ScreenTranslator.hpp"
#include "../Task/Task.hpp"

class ZoneEntity;

class PlayerEntity : public sb::Entity
{
    DEFINE_ENTITY(PlayerEntity,sb::Entity,'BALL')

public:
	enum PlayerState
	{
		kPlayer_Moving,
		kPlayer_Thrown,
		kPlayer_Teleport
	};

	b2Body* getBallBody() const { return m_ballBody.getBody(); }

	void kill(void);
	void fall(void);
	void control(void);
	void bounce(void);
	void shoot(const sf::Vector2f& velocity);

	void resetBodyPosition()
	{
		m_ballBody.resetTransform();
		m_ballBody.updateTransform();
	}

	sb::Task* getTeleportTask() const { return m_teleportTask; }
	void setTeleportTask(sb::Task* val) { m_teleportTask = val; }

	PlayerEntity::PlayerState getPlayerState() const { return m_playerState; }

private:
    virtual void initializeEntity( const TiXmlElement *propertyElement );
    virtual bool handleEvent(const sb::EventData& theevent);
    virtual void update(sf::Time deltaTime);

	//Updates player state
	void updatePlayerState(void);

	//Ball control related functions
	void limitGravitationalVelocity(void);
	void limitHorizontalVelocity(void);
	void limitVerticalVelocity(void);

	//Hooks
	void processContact(const b2Contact* contact, const b2Fixture* contactFixture );
	void processPreSolve(b2Contact* contact,const b2Fixture* target);

	PlayerState m_playerState;
	bool    m_shouldBounce;
	bool	m_moonDead;

	sb::BodyController m_ballBody;
	sb::ScreenTranslator	m_ballTranslator;
	sf::Sprite	m_ballSprite;
	sf::Sound	m_bounceSound;
	sf::Sound	m_throwSound;

	typedef std::list<ZoneEntity*> ZoneEntityList;
	ZoneEntityList m_zoneEntityList;

	sb::Task* m_teleportTask;
};

#endif // PlayerEntity_h__
