#include <cmath>
#include "../Entity/PlayerEntity.hpp"
#include "../Entity/TeleportEntity.hpp"
#include "../Task/TeleportTask.hpp"
#include "../System/PhysicsManager.hpp"
#include "../System/ResourceCache.hpp"
#include "../System/EventManager.hpp"
#include "../System/InputManager.hpp"
#include "../System/SceneManager.hpp"
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
const float MAX_HORI_VELOCITY = 9.0f;
const float MAX_VERTI_VELOCITY = 7.0f;
const float JUMP_SLOPE = 0.25f;

REGISTER_ENTITY( PlayerEntity, "Ball" )

PlayerEntity::PlayerEntity() : Entity(), _ballBody(this), _ballSprite(this),
    _shouldBounce(false),
    _playerState(kPlayer_Moving),
	_bounceSound(nullptr),
	_throwSound(nullptr),
	_zoneEntityList()
{
}

PlayerEntity::~PlayerEntity()
{
	delete _bounceSound;
	delete _throwSound;
}

void PlayerEntity::Update(float deltaTime)
{
	ITransform* starTransform = SceneManager::GetInstance()->FindTransform("Star");
	if(starTransform)
	{
		Vec2D deltaDistance = GetPosition() - starTransform->GetPosition();
		if(Magnitude(deltaDistance) >= 45.0f)
		{
			SetActive(false);

			EventManager::GetInstance()->AbortEvent(Event_NextLevel,true);

			EventData* eventData = new EventData( Event_RestartLevel );
			eventData->QueueEvent(0.5f);
		}
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

	{
		thor::ResourceKey<sf::SoundBuffer> key = thor::Resources::fromFile<sf::SoundBuffer>("Resource/Sounds/jump.wav");
		std::shared_ptr<sf::SoundBuffer> buffer = ResourceCache::GetInstance()->acquire<sf::SoundBuffer>(key);
		_bounceSound = new sf::Sound(*buffer);
		_bounceSound->setVolume(10.f);
	}

	{
		thor::ResourceKey<sf::SoundBuffer> key = thor::Resources::fromFile<sf::SoundBuffer>("Resource/Sounds/throw.wav");
		std::shared_ptr<sf::SoundBuffer> buffer = ResourceCache::GetInstance()->acquire<sf::SoundBuffer>(key);
		_throwSound = new sf::Sound(*buffer);
		_throwSound->setVolume(30.f);
	}

	SceneManager::GetInstance()->RegisterTransform("Player",this);
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

	case Event_EndContact:
		{
			const ContactEventData& contactData = static_cast<const ContactEventData&>(theevent);
			const b2Contact* contactInfo = contactData.GetContact();

			const b2Fixture* target = nullptr;
			if(_ballBody.IsContactRelated(contactInfo,target))
			{
				IPhysics* targetInterface = GetPhysicsInterface(target);
				if(targetInterface->GetEntity()->GetEntityType() == 'ZONE')
				{
					_zoneEntityList.remove(static_cast<ZoneEntity*>(targetInterface->GetEntity()));
				}
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

            if( (slope <= JUMP_SLOPE || _ballBody.GetBody()->GetPosition().y < targetInterface->GetEntity()->GetPosition().y ) && _playerState == kPlayer_Thrown )
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
            else if( slope >= JUMP_SLOPE && (GetPosition().y > targetInterface->GetEntity()->GetPosition().y) )
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
				else if( slope >= JUMP_SLOPE )
				{
					_shouldBounce = true;
				}
				break;
			}

		case 'MVPT':
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

		case 'TLPT':
			{
				if( _playerState == kPlayer_Thrown )
				{
					Fall();
				}

				if(_playerState != kPlayer_Teleport)
				{
					_playerState = kPlayer_Teleport;

					TeleportEntity* tlptEntity = static_cast<TeleportEntity*>(targetInterface->GetEntity());
					TeleportTask* tlptTask = new TeleportTask(1.0f, this, tlptEntity);
					tlptTask->AddTask();
				}

				InputManager::GetInstance()->FeedOutput(0.6f,0.6f);

				break;
			}

		case 'ZONE':
			{
				//implement sort of an interface to keep track of which zone the player entity is in.
				_zoneEntityList.push_back(static_cast<ZoneEntity*>(targetInterface->GetEntity()));
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
		&& slope >= JUMP_SLOPE
		&& target->GetBody()->GetPosition().y < _ballBody.GetBody()->GetPosition().y
		&& _playerState == kPlayer_Moving )
	{
		_shouldBounce = false;
		//contact->SetEnabled(false);
		b2Vec2 unit = target->GetBody()->GetWorldVector(b2Vec2(0,1.0f));
		unit *= THROW_VELOCITY;
		Throw(unit);
	}
}

void PlayerEntity::Control( void )
{
	const bool  leftInput = InputManager::GetInstance()->GetLeftInput(),
				rightInput = InputManager::GetInstance()->GetRightInput();

	/*
	const bool  leftInput = sf::Keyboard::isKeyPressed(sf::Keyboard::Left),
				rightInput = sf::Keyboard::isKeyPressed(sf::Keyboard::Right);
				*/

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
	_playerState = kPlayer_Moving;
	_ballBody.GetBody()->SetGravityScale(6.0f);

	b2Vec2 ballVelocity = _ballBody.GetBody()->GetLinearVelocity();
	if( ballVelocity.y > MAX_VERTI_VELOCITY )
	{
		ballVelocity.y = MAX_VERTI_VELOCITY;
		_ballBody.GetBody()->SetLinearVelocity( ballVelocity );
	}
}

void PlayerEntity::Throw( const b2Vec2& velocity )
{
	_playerState = kPlayer_Thrown;
	_ballBody.GetBody()->SetGravityScale(0.0f);
	_ballBody.GetBody()->SetLinearVelocity(velocity);

	LimitHorizontalVelocity();

	_throwSound->play();

	InputManager::GetInstance()->FeedOutput(0.5f, 0.2f);
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

	_bounceSound->play();

	InputManager::GetInstance()->FeedOutput(0.3f, 0.15f);
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
			if( InputManager::GetInstance()->GetDownInput() /* sf::Keyboard::isKeyPressed(sf::Keyboard::Down) */ )
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
