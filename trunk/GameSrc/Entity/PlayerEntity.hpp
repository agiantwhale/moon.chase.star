#ifndef PLAYERENTITY_HPP
#define PLAYERENTITY_HPP

#include "../Leap/Leap.hpp"
#include <SFML/Audio.hpp>
#include "../Entity/Entity.hpp"
#include "../Entity/Component/BodyWrapper.hpp"
#include "../Entity/Component/SpriteWrapper.hpp"
#include "../Entity/Component/ParticleWrapper.hpp"

class PlayerEntity : public Entity
{
    DEFINE_ENTITY(PlayerEntity,Entity,'BALL')

public:
    virtual void Initialize( const TiXmlElement *propertyElement );
    virtual bool HandleEvent(const EventData& theevent);
    virtual void Update(float deltaTime);

	BodyWrapper& GetBallBody() { return _ballBody; }
	SpriteWrapper& GetBallSprite() { return _ballSprite; }

	void Fall(void);
	void Control(void);
	void Bounce(void);
	void Throw(const b2Vec2& velocity);

private:
	inline bool FingerOnLeft(void)
	{
		Leap::Hand hand = _controller.frame().hands()[0];
		Leap::Vector avgPos = Leap::Vector::zero();

		Leap::FingerList fingers = hand.fingers();
		if(!fingers.empty())
		{
			for(Leap::FingerList::const_iterator iter = fingers.begin(); iter != fingers.end(); iter++)
			{
				avgPos += (*iter).tipPosition();
			}
		}

		return (avgPos.x < -50.f);
	}

	inline bool FingerOnRight(void)
	{
		Leap::Hand hand = _controller.frame().hands()[0];
		Leap::Vector avgPos = Leap::Vector::zero();

		Leap::FingerList fingers = hand.fingers();
		if(!fingers.empty())
		{
			for(Leap::FingerList::const_iterator iter = fingers.begin(); iter != fingers.end(); iter++)
			{
				avgPos += (*iter).tipPosition();
			}
		}

		return (avgPos.x > 50.f);
	}

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

	sf::Keyboard::Key _pressedInput;
	bool				_shouldAcceptInput;	//This is used only during Thrown state!

    BodyWrapper _ballBody;
	SpriteWrapper _ballSprite;
	sf::Sound*	_bounceSound;
	sf::Sound*	_throwSound;

	Leap::Controller _controller;
	//SwipeListener	  _listener;
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
