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

	void Control(void);
	void ProcessContact(const b2Contact* contact, const b2Fixture* contactFixture );
	void ProcessThrow(b2Contact* contact,const b2Fixture* target);
    void Kill(void);
    void Fall(void);
    void Throw(const b2Vec2& velocity);

private:
	enum PlayerState
	{
		kPlayer_Bouncing,
		kPlayer_Moving,
		kPlayer_Thrown
	};

    PlayerState _playerState;
    bool    _shouldBounce;

    BodyWrapper _ballBody;
    SpriteWrapper _ballSprite;

    bool _thrown;
    bool _dead;
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
