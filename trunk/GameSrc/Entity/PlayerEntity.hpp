#ifndef PLAYERENTITY_HPP
#define PLAYERENTITY_HPP

#include <SFML/Audio.hpp>
#include <list>
#include "../Entity/Entity.hpp"
#include "../Entity/ZoneEntity.hpp"
#include "../Entity/Component/BodyWrapper.hpp"
#include "../Entity/Component/SpriteWrapper.hpp"
#include "../Entity/Component/ParticleWrapper.hpp"

class PlayerEntity : public Entity
{
    DEFINE_ENTITY(PlayerEntity,Entity,'BALL')

public:
    virtual void initializeEntity( const TiXmlElement *propertyElement );
    virtual bool handleEvent(const EventData& theevent);
    virtual void update(sf::Time deltaTime);

	void Kill();

	BodyWrapper& GetBallBody() { return _ballBody; }
	SpriteWrapper& GetBallSprite() { return _ballSprite; }

	void Fall(void);
	void Control(void);
	void Bounce(void);
	void Throw(const b2Vec2& velocity);

private:
	//Updates player state
	void UpdatePlayerState(void);

	//Ball control related functions
	void LimitHorizontalVelocity(void);
	void LimitVerticalVelocity(void);

	//Hooks
	void ProcessContact(const b2Contact* contact, const b2Fixture* contactFixture );
	void ProcessPreSolve(b2Contact* contact,const b2Fixture* target);

	enum PlayerState
	{
		kPlayer_Moving,
		kPlayer_Thrown,
		kPlayer_Teleport
	};

    PlayerState _playerState;
    bool    _shouldBounce;

    BodyWrapper _ballBody;
	SpriteWrapper _ballSprite;
	sf::Sound*	_bounceSound;
	sf::Sound*	_throwSound;

	typedef std::list<ZoneEntity*> ZoneEntityList;
	ZoneEntityList _zoneEntityList;
};

class DummyBallEntity : public Entity
{
	DEFINE_ENTITY(DummyBallEntity,Entity,'DMMY')

public:
	virtual void initializeEntity( const TiXmlElement *propertyElement );
	virtual void update(sf::Time deltaTime);

private:
	BodyWrapper _ballBody;
	SpriteWrapper _ballSprite;
};

#endif
