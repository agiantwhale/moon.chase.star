#include <cmath>
#include <Thor/Graphics.hpp>
#include <Thor/Animation.hpp>
#include <Thor/Math.hpp>
#include "../Entity/PlayerEntity.hpp"
#include "../System/PhysicsManager.hpp"
#include "../System/ResourceCache.hpp"
#include "../System/EventManager.hpp"
#include "../Event/ContactEventData.h"
#include "../Event/SolveEventData.h"
#include "CxxTL/tri_logger.hpp"
#include "../Event/AppEventData.hpp"

template <typename T> int sgn(T val)
{
    return (T(0) < val) - (val < T(0));
}

REGISTER_ENTITY( DummyBallEntity, "Dummy")

DummyBallEntity::DummyBallEntity() : Entity(), _ballBody(this), _ballSprite(this)
{

}

DummyBallEntity::~DummyBallEntity()
{

}

void DummyBallEntity::Initialize( const TiXmlElement *propertyElement )
{
	BaseClass::Initialize(propertyElement);

	const float BALL_RADIUS = 0.5f;

	{
		thor::ResourceKey<sf::Texture> key = thor::Resources::fromFile<sf::Texture>("Resource/Ogmo/Entities/Ball.png");
		std::shared_ptr<sf::Texture> texture = ResourceCache::GetInstance()->acquire<sf::Texture>(key);
		sf::Sprite* ballSprite = new sf::Sprite(*texture,sf::IntRect(0,0,32,32));
		ballSprite->setOrigin(sf::Vector2f(BALL_RADIUS*RATIO,BALL_RADIUS*RATIO));
		_ballSprite.SetSprite( ballSprite );
		_ballSprite.RegisterRenderable( 2 );
	}

	//BodyComponent
	{
		b2BodyDef bodyDefinition;
		bodyDefinition.userData = (IPhysics*)this;
		bodyDefinition.position = b2Vec2(GetPosition().x, GetPosition().y);
		bodyDefinition.angle = 0.0f;
		bodyDefinition.fixedRotation = true;
		bodyDefinition.type = b2_dynamicBody;
		bodyDefinition.gravityScale = 6.0f;
		bodyDefinition.bullet = true;

		_ballBody.CreateBody( bodyDefinition );

		b2CircleShape circle;
		circle.m_radius = BALL_RADIUS;

		b2FixtureDef fixtureDefinition;
		fixtureDefinition.restitution = 0.7f;
		fixtureDefinition.friction = 0.1f;
		fixtureDefinition.density = 1.0f;
		fixtureDefinition.shape = &circle;

		_ballBody.CreateFixture( fixtureDefinition, "Ball" );

		_ballBody.ResetTransform();
	}
}

void DummyBallEntity::Update( float deltaTime )
{
	if(GetPosition().y <= -13.0f)
	{
		Release();
	}
}

//PlayerEntity related constants
const float BALL_RADIUS = 0.5f;
const float KILL_TIME = 1.5f;
const float JUMP_IMPULSE = 18.0f;
const float MOVE_IMPULSE = 0.5f;
const float MAX_HORI_VELOCITY = 15.0f;
const float MAX_VERTI_VELOCITY = 15.0f;

REGISTER_ENTITY( PlayerEntity, "Ball" )

PlayerEntity::PlayerEntity() : Entity(), _ballBody(this), _ballSprite(this), _ballParticle(this),
    _shouldBounce(false),
	_shouldAcceptInput(true),
    _playerState(kPlayer_Moving),
	emitter(nullptr)
{
	AddEventListenType(Event_App);
}

PlayerEntity::~PlayerEntity()
{
	RemoveEventListenType(Event_App);
}

void PlayerEntity::Update(float deltaTime)
{
	Vec2D position = WorldToScreen(GetPosition());
	Vec2D velocity = _ballBody.GetBody()->GetLinearVelocity();
	velocity *= RATIO;
	velocity.y *= -1.f;
	velocity *= 0.1f;
	
	emitter->setParticleRotationSpeed(20.f);
	emitter->setParticleVelocity(thor::Distributions::deflect(velocity, 30.f) );
	emitter->setParticlePosition(thor::Distributions::circle(position,BALL_RADIUS*RATIO * 0.2));

	_ballParticle.Update(deltaTime);

	//Loses the game.
    if( GetPosition().y < -(SCREENHEIGHT * UNRATIO * 0.5f + 5.0f) )
    {
		SetActive(false);

		EventManager::GetInstance()->AbortEvent(Event_NextLevel,true);

		EventData* eventData = new EventData( Event_RestartLevel );
		eventData->QueueEvent(0.5f);
    }
}

void PlayerEntity::Initialize( const TiXmlElement *propertyElement )
{
    BaseClass::Initialize(propertyElement);

    {
		thor::ResourceKey<sf::Texture> key = thor::Resources::fromFile<sf::Texture>("Resource/Ogmo/Entities/Ball.png");
		std::shared_ptr<sf::Texture> texture = ResourceCache::GetInstance()->acquire<sf::Texture>(key);
		sf::Sprite* ballSprite = new sf::Sprite(*texture, sf::IntRect(0,0,32,32));
        ballSprite->setOrigin(sf::Vector2f(BALL_RADIUS*RATIO,BALL_RADIUS*RATIO));
        _ballSprite.SetSprite( ballSprite );
        _ballSprite.RegisterRenderable( 2 );
    }

	{
		thor::ResourceKey<sf::Texture> key = thor::Resources::fromFile<sf::Texture>("Resource/Particles/whiteSpark.png");
		std::shared_ptr<sf::Texture> texture = ResourceCache::GetInstance()->acquire<sf::Texture>(key);
		thor::ParticleSystem* particleSystem = new thor::ParticleSystem(texture);

		// Create emitter
		emitter = thor::UniversalEmitter::create();
		emitter->setEmissionRate(200.f);
		emitter->setParticleLifetime(sf::seconds(3.f));
		emitter->setParticlePosition(WorldToScreen(GetPosition()));

		particleSystem->addEmitter(emitter);

		// Build color gradient (green -> teal -> blue)
		/*
		thor::ColorGradient gradient = thor::createGradient
			(sf::Color(255, 255, 255))		(1)
			(sf::Color(0, 0, 0));

		// Create color and fade in/out animations
		thor::ColorAnimation colorizer(gradient);
		*/
		thor::FadeAnimation fader(0.f, 1.f);

		// Add particle affectors
		//particleSystem->addAffector( thor::AnimationAffector::create(colorizer) );
		particleSystem->addAffector( thor::AnimationAffector::create(fader) );
		particleSystem->addAffector( thor::TorqueAffector::create(100.f) );
		//particleSystem->addAffector( thor::ForceAffector::create(sf::Vector2f(0.f, 9.8f * RATIO * 0.01f))  );

		_ballParticle.SetParticleSystem(particleSystem);
		//_ballParticle.RegisterRenderable(5);
	}

    //BodyComponent
    {
        b2BodyDef bodyDefinition;
        bodyDefinition.userData = (IPhysics*)this;
        bodyDefinition.position = b2Vec2(GetPosition().x, GetPosition().y);
        bodyDefinition.angle = 0.0f;
        bodyDefinition.fixedRotation = true;
        bodyDefinition.type = b2_dynamicBody;
        bodyDefinition.gravityScale = 6.0f;
        bodyDefinition.bullet = true;

        _ballBody.CreateBody( bodyDefinition );

        b2CircleShape circle;
        circle.m_radius = BALL_RADIUS;

        b2FixtureDef fixtureDefinition;
        fixtureDefinition.density = 1.0f;
		fixtureDefinition.restitution = 0.5f;
        fixtureDefinition.shape = &circle;

        _ballBody.CreateFixture( fixtureDefinition, "Ball" );

        _ballBody.ResetTransform();
    }
}

bool PlayerEntity::HandleEvent(const EventData& theevent)
{
    switch (theevent.GetEventType())
    {
    case Event_BeginContact:
		{
			const ContactEventData& contactData = static_cast<const ContactEventData&>(theevent);
			const b2Contact* contactInfo = contactData.GetContact();

			const b2Fixture* target = nullptr;
			if(_ballBody.IsContactRelated(contactInfo,target))
			{
				ProcessContact(contactInfo,target);
			}

			break;
		}

    case Event_Simulate:
		{
			UpdatePlayerState();
			break;
		}

	case Event_PreSolve:
		{
			const PreSolveEventData& eventData = static_cast<const PreSolveEventData&>(theevent);
			b2Contact* contactInfo = eventData.GetContact();

			const b2Fixture* target = nullptr;
			if(_ballBody.IsContactRelated(contactInfo,target))
			{
				ProcessPreSolve(contactInfo,target);
			}

			break;
		}

	case Event_App:
		{
			if(_playerState != kPlayer_Thrown) break;

			const AppEventData& eventData = static_cast<const AppEventData&>(theevent);

			if(eventData.GetAppEvent().type == sf::Event::KeyReleased &&
			   eventData.GetAppEvent().key.code == _pressedInput )
			{
				_shouldAcceptInput = true;
			}

			break;
		}

    default:
        break;
    }

    return false;
}

void PlayerEntity::ProcessContact(const b2Contact* contact, const b2Fixture* contactFixture )
{
    b2WorldManifold manifold;
    contact->GetWorldManifold(&manifold);

    b2Vec2 normal = manifold.normal;
    float slope = std::abs(normal.y/normal.x);

    IPhysics *targetInterface = GetPhysicsInterface(contactFixture);

    if(targetInterface)
    {
        switch(targetInterface->GetEntity()->GetEntityType())
        {
        case 'THRW':
        {
			_shouldBounce = false;

            if( (slope <= 0.25f || _ballBody.GetBody()->GetPosition().y < targetInterface->GetEntity()->GetPosition().y ) && _playerState == kPlayer_Thrown )
            {
                Fall();
            }
            break;
        }

        case 'HULL':
        {
            if( _playerState == kPlayer_Thrown )
            {
                Fall();
            }
            else if( slope >= 10.0f )
            {
                _shouldBounce = true;
            }
            break;
        }

		case 'BLCK':
			{
				if( _playerState == kPlayer_Thrown )
				{
					Fall();
				}
				else if( slope >= 10.0f )
				{
					_shouldBounce = true;
				}
				break;
			}

        default:
        {
            _shouldBounce = false;
            break;
        }
        }
    }
}

void PlayerEntity::ProcessPreSolve( b2Contact* contact,const b2Fixture* target )
{
	const float THROW_VELOCITY = 40.0f;

	b2WorldManifold manifold;
	contact->GetWorldManifold(&manifold);

	b2Vec2 normal = manifold.normal;
	float slope = std::abs(normal.y/normal.x);

	IPhysics *physicsInterface = GetPhysicsInterface(target);

	if(physicsInterface && physicsInterface->GetEntity()->GetEntityType() == 'THRW'
		&& slope >= 10.0f
		&& target->GetBody()->GetPosition().y < _ballBody.GetBody()->GetPosition().y )
	{
		_shouldBounce = false;
		contact->SetEnabled(false);
		b2Vec2 unit = target->GetBody()->GetWorldVector(b2Vec2(0,1.0f));
		unit *= THROW_VELOCITY;
		Throw(unit);
	}
}

void PlayerEntity::Control( void )
{
	if(!_shouldAcceptInput) return;

	const bool  leftInput = sf::Keyboard::isKeyPressed(sf::Keyboard::Left),
				rightInput = sf::Keyboard::isKeyPressed(sf::Keyboard::Right);

	b2Vec2 ballVelocity = _ballBody.GetBody()->GetLinearVelocity();
	b2Vec2 ballPosition = _ballBody.GetBody()->GetPosition();

	if( leftInput && ballVelocity.x > -MAX_HORI_VELOCITY )
	{
		_ballBody.GetBody()->ApplyLinearImpulse( b2Vec2( -MOVE_IMPULSE, 0 ), ballPosition );
	}

	if( rightInput && ballVelocity.x < MAX_HORI_VELOCITY )
	{
		_ballBody.GetBody()->ApplyLinearImpulse( b2Vec2( MOVE_IMPULSE, 0 ), ballPosition );
	}
}

void PlayerEntity::Fall( void )
{
	_shouldAcceptInput = true;
	_playerState = kPlayer_Moving;
	_ballBody.GetBody()->SetGravityScale(6.0f);

	const float MAX_VELOCITY = 15.0f;
	b2Vec2 ballVelocity = _ballBody.GetBody()->GetLinearVelocity();
	if( ballVelocity.y > MAX_VELOCITY )
	{
		ballVelocity.y = MAX_VELOCITY;
		_ballBody.GetBody()->SetLinearVelocity( ballVelocity );
	}
}

void PlayerEntity::Throw( const b2Vec2& velocity )
{
	const bool leftInput = sf::Keyboard::isKeyPressed(sf::Keyboard::Left),
			   rightInput = sf::Keyboard::isKeyPressed(sf::Keyboard::Right);

	_shouldAcceptInput = false;

	if(leftInput)
	{
		_pressedInput = sf::Keyboard::Left;
	}

	if(rightInput)
	{
		_pressedInput = sf::Keyboard::Right;
	}
	
	if( !leftInput && !rightInput )
	{
		_shouldAcceptInput = true;
	}

	_playerState = kPlayer_Thrown;
	_ballBody.GetBody()->SetGravityScale(0.0f);
	_ballBody.GetBody()->SetLinearVelocity(velocity);
}

void PlayerEntity::LimitHorizontalVelocity()
{
	b2Vec2 ballVelocity = _ballBody.GetBody()->GetLinearVelocity();

	if( std::abs( ballVelocity.x ) > MAX_HORI_VELOCITY )
	{
		ballVelocity.x = sgn<float>(ballVelocity.x) * MAX_HORI_VELOCITY;
		_ballBody.GetBody()->SetLinearVelocity( ballVelocity );
	}
}

void PlayerEntity::LimitVerticalVelocity()
{
	b2Vec2 ballVelocity = _ballBody.GetBody()->GetLinearVelocity();

	//Should this only limit when the velocity is bigger than MAX_VERTI_VELOCITY when going up?
	if( std::abs( ballVelocity.y ) > MAX_VERTI_VELOCITY )
	{
		ballVelocity.y = sgn<float>(ballVelocity.y) * MAX_VERTI_VELOCITY;
		_ballBody.GetBody()->SetLinearVelocity( ballVelocity );
	}
}

void PlayerEntity::Bounce( void )
{
	_shouldBounce = false;

	b2Vec2 ballVelocity = _ballBody.GetBody()->GetLinearVelocity();
	b2Vec2 ballPosition = _ballBody.GetBody()->GetPosition();

	_ballBody.GetBody()->SetLinearVelocity( b2Vec2(ballVelocity.x, 0) );
	_ballBody.GetBody()->SetTransform(ballPosition + b2Vec2(0,0.1f), _ballBody.GetBody()->GetAngle());
	_ballBody.GetBody()->ApplyLinearImpulse(b2Vec2(0,JUMP_IMPULSE), _ballBody.GetBody()->GetPosition());
}

void PlayerEntity::UpdatePlayerState( void )
{
	switch(_playerState)
	{
	case kPlayer_Moving:
		{
			Control();

			if(_shouldBounce)
			{
				Bounce();
			}

			LimitHorizontalVelocity();

			break;
		}

	case kPlayer_Thrown:
		{
			const bool leftInput = sf::Keyboard::isKeyPressed(sf::Keyboard::Left),
					   rightInput = sf::Keyboard::isKeyPressed(sf::Keyboard::Right);

			if( _shouldAcceptInput && (leftInput || rightInput) )
			{
				Fall();
			}
			else
			{
				LimitVerticalVelocity();
			}

			LimitHorizontalVelocity();

			break;
		}
	}
}
