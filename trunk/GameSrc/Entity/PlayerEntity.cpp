#include <cmath>
#include <Thor/Vectors.hpp>
#include "../Base/Math.hpp"
#include "../Helper/Conversion.hpp"
#include "PlayerEntity.hpp"
#include "TeleportEntity.hpp"
#include "ThrowEntity.hpp"
#include "StarEntity.hpp"
#include "ZoneEntity.hpp"
#include "../Task/TeleportTask.hpp"
#include "../Physics/PhysicsManager.hpp"
#include "../System/ResourceCache.hpp"
#include "../Event/EventManager.hpp"
#include "../System/InputManager.hpp"
#include "../System/SceneManager.hpp"
#include "../System/GraphicsManager.hpp"
#include "../Event/ContactEventData.hpp"
#include "../Event/SolveEventData.hpp"
#include "../Event/AppEventData.hpp"

//PlayerEntity related constants
const float BALL_RADIUS = 0.5f;
const float KILL_TIME = 1.5f;
const float JUMP_IMPULSE = 18.0f;
const float MOVE_IMPULSE = 0.5f;
const float MAX_HORI_VELOCITY = 9.0f;
const float MAX_VERTI_VELOCITY = 7.0f;
const float JUMP_SLOPE = 0.25f;

REGISTER_ENTITY( PlayerEntity, "Ball" )

PlayerEntity::PlayerEntity() : Entity(), m_ballBody(*this), m_ballSprite(),
    m_shouldBounce(false),
    m_playerState(kPlayer_Moving),
	m_bounceSound(),
	m_throwSound(),
	m_zoneEntityList()
{
}

PlayerEntity::~PlayerEntity()
{
	sb::GraphicsManager::getInstance()->removeDrawable(m_ballSprite,2);
}

void PlayerEntity::update(sf::Time deltaTime)
{
	sf::Transformable* starTransform = sb::SceneManager::getInstance()->getStarEntity();
	if(starTransform)
	{
		sf::Vector2f deltaDistance = getPosition() - starTransform->getPosition();
		if(thor::length<float>(deltaDistance) >= 45.0f)
		{
			setActive(false);

			kill();
		}
	}
}

void PlayerEntity::initializeEntity( const TiXmlElement *propertyElement )
{
    BaseClass::initializeEntity(propertyElement);


    {
		thor::ResourceKey<sf::Texture> key = thor::Resources::fromFile<sf::Texture>("Resource/Ogmo/Entities/Ball.png");
		std::shared_ptr<sf::Texture> texture = sb::ResourceCache::getInstance()->acquire<sf::Texture>(key);
		m_ballSprite.setTexture(*texture);
		m_ballSprite.setTextureRect(sf::IntRect(0,0,32,32));

		sb::GraphicsManager::getInstance()->addDrawable(m_ballSprite,2);
    }

    //BodyComponent
    {
        b2BodyDef bodyDefinition;
        bodyDefinition.userData = (Entity*)this;
        bodyDefinition.position = ToVector(getPosition());
        bodyDefinition.angle = 0.0f;
        bodyDefinition.fixedRotation = true;
        bodyDefinition.type = b2_dynamicBody;
        bodyDefinition.gravityScale = 6.0f;
        bodyDefinition.bullet = true;

		b2Body* ballBody = sb::PhysicsManager::getInstance()->getPhysicsWorld()->CreateBody(&bodyDefinition);

        b2CircleShape circle;
        circle.m_radius = BALL_RADIUS;

        b2FixtureDef fixtureDefinition;
        fixtureDefinition.density = 1.0f;
		fixtureDefinition.restitution = 0.5f;
        fixtureDefinition.shape = &circle;

       ballBody->CreateFixture(&fixtureDefinition);

	   m_ballBody.setBody(ballBody);
    }

	{
		thor::ResourceKey<sf::SoundBuffer> key = thor::Resources::fromFile<sf::SoundBuffer>("Resource/Sounds/jump.wav");
		std::shared_ptr<sf::SoundBuffer> buffer = sb::ResourceCache::getInstance()->acquire<sf::SoundBuffer>(key);
		m_bounceSound.setBuffer(*buffer);
		m_bounceSound.setVolume(10.f);
	}

	{
		thor::ResourceKey<sf::SoundBuffer> key = thor::Resources::fromFile<sf::SoundBuffer>("Resource/Sounds/throw.wav");
		std::shared_ptr<sf::SoundBuffer> buffer = sb::ResourceCache::getInstance()->acquire<sf::SoundBuffer>(key);
		m_throwSound.setBuffer(*buffer);
		m_throwSound.setVolume(50.f);
	}

	sb::SceneManager::getInstance()->setPlayerEntity(this);
}

bool PlayerEntity::handleEvent(const sb::EventData& theevent)
{
    switch (theevent.getEventType())
    {
    case Event_BeginContact:
		{
			const sb::ContactEventData& contactData = static_cast<const sb::ContactEventData&>(theevent);
			const b2Contact* contactInfo = contactData.getContact();

			const b2Fixture* target = nullptr;
			if(m_ballBody.checkContact(contactInfo,target))
			{
				processContact(contactInfo,target);
			}

			break;
		}

	case Event_EndContact:
		{
			const sb::ContactEventData& contactData = static_cast<const sb::ContactEventData&>(theevent);
			const b2Contact* contactInfo = contactData.getContact();

			const b2Fixture* target = nullptr;
			if( m_ballBody.checkContact(contactInfo,target))
			{
				sb::Entity* entity = static_cast<sb::Entity*>(target->GetBody()->GetUserData());
				ZoneEntity* zoneEntity = sb::entity_cast<ZoneEntity>(entity);

				if(zoneEntity)
				{
					m_zoneEntityList.remove(zoneEntity);
				}
			}

			break;
		}

    case Event_Simulate:
		{
			updatePlayerState();
			break;
		}

	case Event_PreSolve:
		{
			const sb::PreSolveEventData& eventData = static_cast<const sb::PreSolveEventData&>(theevent);
			b2Contact* contactInfo = eventData.GetContact();

			const b2Fixture* target = nullptr;
			if(m_ballBody.checkContact(contactInfo,target))
			{
				processPreSolve(contactInfo,target);
			}
			
			break;
		}

    default:
        break;
    }

    return false;
}

void PlayerEntity::processContact(const b2Contact* contact, const b2Fixture* contactFixture )
{
    b2WorldManifold manifold;
    contact->GetWorldManifold(&manifold);

    b2Vec2 normal = manifold.normal;
    float slope = std::abs(normal.y/normal.x);

    sb::Entity* entity = static_cast<sb::Entity*>(contactFixture->GetBody()->GetUserData());

    if(entity)
    {
        switch(entity->getEntityType())
        {
        case 'THRW':
        {
			m_shouldBounce = false;

            if( (slope < JUMP_SLOPE || getPosition().y < entity->getPosition().y ) && m_playerState == kPlayer_Thrown )
            {
                fall();
            }
            break;
        }

        case 'HULL':
        {
            if( m_playerState == kPlayer_Thrown )
            {
                fall();
            }
            else if( slope >= JUMP_SLOPE && (getPosition().y > entity->getPosition().y) )
            {
                m_shouldBounce = true;
            }
            break;
        }

		case 'BLCK':
			{
				if( m_playerState == kPlayer_Thrown )
				{
					fall();
				}
				else if( slope >= JUMP_SLOPE )
				{
					m_shouldBounce = true;
				}
				break;
			}

		case 'MVPT':
			{
				if( m_playerState == kPlayer_Thrown )
				{
					fall();
				}
				else if( slope >= JUMP_SLOPE )
				{
					m_shouldBounce = true;
				}
				break;
			}

		case 'TLPT':
			{
				if( m_playerState == kPlayer_Thrown )
				{
					fall();
				}

				if(m_playerState != kPlayer_Teleport)
				{
					m_playerState = kPlayer_Teleport;

					TeleportEntity* tlptEntity = sb::entity_cast<TeleportEntity>(entity);
					TeleportTask* tlptTask = new TeleportTask(sf::seconds(1.f), this, tlptEntity);
					tlptTask->addTask();
				}

				sb::InputManager::getInstance()->feedOutput(0.6f,sf::seconds(.6f));

				break;
			}

		case 'ZONE':
			{
				//implement sort of an interface to keep track of which zone the player entity is in.
				m_zoneEntityList.push_back(sb::entity_cast<ZoneEntity>(entity));
				break;
			}

        default:
        {
            m_shouldBounce = false;
            break;
        }
        }
    }
}

void PlayerEntity::processPreSolve( b2Contact* contact,const b2Fixture* target )
{
	const float THROW_VELOCITY = 40.0f;

	b2WorldManifold manifold;
	contact->GetWorldManifold(&manifold);

	b2Vec2 normal = manifold.normal;
	float slope = std::abs(normal.y/normal.x);

	sb::Entity* entity = static_cast<sb::Entity*>(target->GetBody()->GetUserData());
	ThrowEntity* throwEntity = sb::entity_cast<ThrowEntity>(entity);

	if( throwEntity
		&& slope >= JUMP_SLOPE
		&& throwEntity->getPosition().y < getPosition().y
		&& m_playerState == kPlayer_Moving )
	{
		m_shouldBounce = false;
		//contact->SetEnabled(false);
		sf::Vector2f throwVelocity = ToVector(target->GetBody()->GetWorldVector(b2Vec2(0,1.0f))) * THROW_VELOCITY;
		shoot(throwVelocity);
	}
}

void PlayerEntity::control( void )
{
	const bool  leftInput = sb::InputManager::getInstance()->getLeftInput(),
				rightInput = sb::InputManager::getInstance()->getRightInput();

	/*
	const bool  leftInput = sf::Keyboard::isKeyPressed(sf::Keyboard::Left),
				rightInput = sf::Keyboard::isKeyPressed(sf::Keyboard::Right);
				*/

	b2Vec2 ballVelocity = m_ballBody.getBody()->GetLinearVelocity();
	b2Vec2 ballPosition = m_ballBody.getBody()->GetPosition();

	if( leftInput && ballVelocity.x > -MAX_HORI_VELOCITY )
	{
		m_ballBody.getBody()->ApplyLinearImpulse( b2Vec2( -MOVE_IMPULSE, 0 ), ballPosition );
	}

	if( rightInput && ballVelocity.x < MAX_HORI_VELOCITY )
	{
		m_ballBody.getBody()->ApplyLinearImpulse( b2Vec2( MOVE_IMPULSE, 0 ), ballPosition );
	}
}

void PlayerEntity::fall( void )
{
	m_playerState = kPlayer_Moving;
	m_ballBody.getBody()->SetGravityScale(6.0f);

	b2Vec2 ballVelocity = m_ballBody.getBody()->GetLinearVelocity();
	if( ballVelocity.y > MAX_VERTI_VELOCITY )
	{
		ballVelocity.y = MAX_VERTI_VELOCITY;
		m_ballBody.getBody()->SetLinearVelocity( ballVelocity );
	}
}

void PlayerEntity::shoot( const sf::Vector2f& velocity )
{
	m_playerState = kPlayer_Thrown;
	m_ballBody.getBody()->SetGravityScale(0.0f);
	m_ballBody.getBody()->SetLinearVelocity(ToVector(velocity));

	limitHorizontalVelocity();

	m_throwSound.play();

	sb::InputManager::getInstance()->feedOutput(0.5f, sf::seconds(0.2f));
}

void PlayerEntity::limitHorizontalVelocity()
{
	b2Vec2 ballVelocity = m_ballBody.getBody()->GetLinearVelocity();

	if( std::abs( ballVelocity.x ) > MAX_HORI_VELOCITY )
	{
		ballVelocity.x = sb::signum<float>(ballVelocity.x) * MAX_HORI_VELOCITY;
		m_ballBody.getBody()->SetLinearVelocity( ballVelocity );
	}
}

void PlayerEntity::limitVerticalVelocity()
{
	b2Vec2 ballVelocity = m_ballBody.getBody()->GetLinearVelocity();

	//Should this only limit when the velocity is bigger than MAX_VERTI_VELOCITY when going up?
	if( std::abs( ballVelocity.y ) > MAX_VERTI_VELOCITY )
	{
		ballVelocity.y = sb::signum<float>(ballVelocity.y) * MAX_VERTI_VELOCITY;
		m_ballBody.getBody()->SetLinearVelocity( ballVelocity );
	}
}

void PlayerEntity::bounce( void )
{
	m_shouldBounce = false;

	b2Vec2 ballVelocity = m_ballBody.getBody()->GetLinearVelocity();
	b2Vec2 ballPosition = m_ballBody.getBody()->GetPosition();

	m_ballBody.getBody()->SetLinearVelocity( b2Vec2(ballVelocity.x, 0) );
	m_ballBody.getBody()->SetTransform(ballPosition + b2Vec2(0,0.1f), m_ballBody.getBody()->GetAngle());
	m_ballBody.getBody()->ApplyLinearImpulse(b2Vec2(0,JUMP_IMPULSE), m_ballBody.getBody()->GetPosition());

	m_bounceSound.play();

	sb::InputManager::getInstance()->feedOutput(0.3f, sf::seconds(0.15f));
}

void PlayerEntity::updatePlayerState( void )
{
	switch(m_playerState)
	{
	case kPlayer_Moving:
		{
			control();

			if(m_shouldBounce)
			{
				bounce();
			}

			limitHorizontalVelocity();

			break;
		}

	case kPlayer_Thrown:
		{
			if( sb::InputManager::getInstance()->getDownInput() /* sf::Keyboard::isKeyPressed(sf::Keyboard::Down) */ )
			{
				fall();
			}
			else
			{
				limitVerticalVelocity();
			}

			limitHorizontalVelocity();

			break;
		}
	}
}

void PlayerEntity::kill()
{
	sb::EventManager::getInstance()->abortEvent(Event_NextLevel,true);

	sb::EventData* eventData = new sb::EventData( Event_RestartLevel );
	eventData->queueEvent(sf::seconds(0.5f));
}
