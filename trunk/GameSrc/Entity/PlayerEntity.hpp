#ifndef PLAYERENTITY_HPP
#define PLAYERENTITY_HPP

#include "../Entity/Entity.hpp"
#include "../Entity/Component/BodyWrapper.hpp"
#include "../Entity/Component/SpriteWrapper.hpp"

class PlayerEntity : public Entity
{
    DEFINE_ENTITY(PlayerEntity,Entity,'BALL')

public:
    enum PlayerState
    {
        kPlayer_Bouncing,
        kPlayer_Moving,
        kPlayer_Thrown
    };

    virtual void Initialize( const TiXmlElement *propertyElement );
	virtual bool HandleEvent(const EventData& theevent);
    virtual void ProcessContact(const b2Contact* contact, const b2Fixture* contactFixture );
    virtual void Control(void);
    virtual void Update(float deltaTime);

    void Kill(void);
    void Fall(void)
    {
        _playerState = kPlayer_Moving;
        if( kPlayer_Thrown )
        {
			_ballBody.GetBody()->SetGravityScale(6.0f);
        }
    }

    inline void Throw(const b2Vec2& velocity)
    {
        _playerState = kPlayer_Thrown;
        _ballBody.GetBody()->SetGravityScale(0.0f);
        _ballBody.GetBody()->SetLinearVelocity(velocity);
    }
private:
    PlayerState _playerState;
    bool    _shouldBounce;

	BodyWrapper _ballBody;
	SpriteWrapper _ballSprite;

    bool _thrown;
    bool _dead;
};

#endif
