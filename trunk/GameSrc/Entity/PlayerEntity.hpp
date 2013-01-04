#ifndef PLAYERENTITY_HPP
#define PLAYERENTITY_HPP

#include "../Entity/Entity.hpp"
#include "../Entity/Component/BodyWrapper.hpp"
#include "../Entity/Component/SpriteWrapper.hpp"

class PlayerEntity : public Entity
{
    DEFINE_ENTITY(PlayerEntity,Entity,'BALL')

public:
    virtual void Initialize( const TiXmlElement *propertyElement );
    virtual bool HandleEvent(const EventData& theevent);
    virtual void Update(float deltaTime);

private:
	//Updates player state
	void UpdatePlayerState(void);

	//Ball control related functions
	void Control(void);
	void Bounce(void);
	void Fall(void);
	void Throw(const b2Vec2& velocity);
	void LimitHorizontalVelocity(void);
	void LimitVerticalVelocity(void);

	//Hooks
	void ProcessContact(const b2Contact* contact, const b2Fixture* contactFixture );
	void ProcessPreSolve(b2Contact* contact,const b2Fixture* target);

	enum PlayerState
	{
		kPlayer_Moving,
		kPlayer_Thrown
	};

    PlayerState _playerState;
    bool    _shouldBounce;

	sf::Keyboard::Key _pressedInput;
	bool				_shouldAcceptInput;	//This is used only during Thrown state!

    BodyWrapper _ballBody;
    SpriteWrapper _ballSprite;
};

class DummyBallEntity : public Entity
{
	DEFINE_ENTITY(DummyBallEntity,Entity,'DMMY')

public:
	virtual void Initialize( const TiXmlElement *propertyElement );
	virtual void Update(float deltaTime);

private:
	BodyWrapper _ballBody;
	SpriteWrapper _ballSprite;
};

#endif
