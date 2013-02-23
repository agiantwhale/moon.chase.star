#include <cmath>
#include <Thor/Vectors.hpp>
#include "../Base/Math.hpp"
#include "../Helper/Conversion.hpp"
#include "PlayerEntity.hpp"
#include "TeleportEntity.hpp"
#include "ThrowEntity.hpp"
#include "StarEntity.hpp"
#include "ZoneEntity.hpp"
#include "GravityEntity.hpp"
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
const float MAX_MOVE_VELOCITY = 9.0f;
const float JUMP_SLOPE = 0.25f;
const float JUMP_SLOPE_STRICT = 0.001f;
const float THROW_VELOCITY = 40.0f;

REGISTER_ENTITY( PlayerEntity, "Ball" )

PlayerEntity::PlayerEntity() : Entity(), m_ballBody(*this), m_ballSprite(),
	m_ballTranslator(*this),
    m_shouldBounce(false),
	m_moonDead(false),
    m_playerState(kPlayer_Moving),
	m_bounceSound(),
	m_throwSound(),
	m_zoneEntityList(),
	m_teleportTask(nullptr)
{
	addEventListenType(Event_BeginContact);
	addEventListenType(Event_EndContact);
	addEventListenType(Event_PreSolve);
	addEventListenType(Event_Simulate);
}

PlayerEntity::~PlayerEntity()
{
	removeEventListenType(Event_BeginContact);
	removeEventListenType(Event_EndContact);
	removeEventListenType(Event_PreSolve);
	removeEventListenType(Event_Simulate);

	sb::GraphicsManager::getInstance()->removeDrawable(m_ballSprite,2);
	sb::PhysicsManager::getInstance()->removeSimulatable(&m_ballBody);
}

void PlayerEntity::update(sf::Time deltaTime)
{
	/*
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
	*/

	m_ballTranslator.translate(m_ballSprite);
}

void PlayerEntity::initializeEntity( const TiXmlElement *propertyElement )
{
    BaseClass::initializeEntity(propertyElement);

    {
		thor::ResourceKey<sf::Texture> key = thor::Resources::fromFile<sf::Texture>("Resource/Ogmo/Entities/Ball.png");
		std::shared_ptr<sf::Texture> texture = sb::ResourceCache::getInstance()->acquire<sf::Texture>(key);
		m_ballSprite.setTexture(*texture);
		m_ballSprite.setTextureRect(sf::IntRect(0,0,32,32));
		m_ballSprite.setOrigin(sf::Vector2f(16.f,16.f));

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

		sb::PhysicsManager::getInstance()->addSimulatable(&m_ballBody);
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
	if(BaseClass::handleEvent(theevent))
	{
		return true;
	}

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
			/*
			const sb::PreSolveEventData& eventData = static_cast<const sb::PreSolveEventData&>(theevent);
			b2Contact* contactInfo = eventData.GetContact();

			const b2Fixture* target = nullptr;
			if(m_ballBody.checkContact(contactInfo,target))
			{
				processPreSolve(contactInfo,target);
			}
			*/
			
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

    sb::Entity* entity = sb::getOwnerEntity(contactFixture);

	if(entity == nullptr)
	{
		return;
	}

	const sb::PhysicsManager::GravityDirection gravityDirection = sb::PhysicsManager::getInstance()->getGravityDirection();

	/*
	if( (gravityDirection == sb::PhysicsManager::Gravity_Up || gravityDirection == sb::PhysicsManager::Gravity_Down) && slope >= JUMP_SLOPE )
	{
		m_shouldBounce = true;
	}

	if( (gravityDirection == sb::PhysicsManager::Gravity_Left || gravityDirection == sb::PhysicsManager::Gravity_Right) && slope <= 1/JUMP_SLOPE )
	{
		m_shouldBounce = true;
	}
	*/

	switch (gravityDirection)
	{
	case sb::PhysicsManager::Gravity_Down:
		{
			if( slope >= JUMP_SLOPE && getPosition().y > entity->getPosition().y)
			{
				m_shouldBounce = true;
			}

			break;
		}

	case sb::PhysicsManager::Gravity_Up:
		{
			if( slope >= JUMP_SLOPE && getPosition().y < entity->getPosition().y)
			{
				m_shouldBounce = true;
			}

			break;
		}

	case sb::PhysicsManager::Gravity_Left:
		{
			if( slope <= 1/JUMP_SLOPE && getPosition().x > entity->getPosition().x)
			{
				m_shouldBounce = true;
			}

			break;
		}

	case sb::PhysicsManager::Gravity_Right:
		{
			if( slope <= 1/JUMP_SLOPE && getPosition().x < entity->getPosition().x)
			{
				m_shouldBounce = true;
			}

			break;
		}
	}

	switch(entity->getEntityType())
	{
			case 'THRW':
			{
				//m_shouldBounce = false;
				//m_throwSound.play();

				b2Vec2 contactPos = contactFixture->GetBody()->GetLocalPoint(m_ballBody.getBody()->GetPosition());
				ThrowEntity* throwEntity = sb::entity_cast<ThrowEntity>(entity);

				if( std::abs(normal.x) <= JUMP_SLOPE_STRICT ||
					(std::abs(normal.y) <= JUMP_SLOPE_STRICT && contactPos.y > 0.f))
				{
					m_shouldBounce = false;
					m_throwSound.play();
					//contact->SetEnabled(false);
					sf::Vector2f throwVelocity = ToVector(contactFixture->GetBody()->GetWorldVector(b2Vec2(0,1.0f))) * THROW_VELOCITY;
					shoot(throwVelocity);
				}
				else if( (std::abs(normal.y) <= JUMP_SLOPE_STRICT && contactPos.y < 0.f) )
				{
					m_shouldBounce = false;
					fall();
				}

				/*
				b2Vec2 contactPos = contactFixture->GetBody()->GetLocalPoint(m_ballBody.getBody()->GetPosition());
				if(contactPos.y > 0.f)
				{
					m_shouldBounce = false;
					m_throwSound.play();
				}
				*/

				/*
				if( (slope < JUMP_SLOPE || getPosition().y < entity->getPosition().y ) && m_playerState == kPlayer_Thrown )
				{
					fall();
				}
				*/

				break;
			}

			case 'HULL':
			{
				if( m_playerState == kPlayer_Thrown )
				{
					m_shouldBounce = false;
					fall();
				}
			
				/*
				else if( slope >= JUMP_SLOPE && (getPosition().y > entity->getPosition().y) )
				{
					m_shouldBounce = true;
				}
				*/

				break;
			}

			case 'BLCK':
				{
					if( m_playerState == kPlayer_Thrown )
					{
						m_shouldBounce = false;
						fall();
					}

					/*
					else if( slope >= JUMP_SLOPE )
					{
						m_shouldBounce = true;
					}
					*/

					break;
				}

			case 'MVPT':
				{
					if( m_playerState == kPlayer_Thrown )
					{
						m_shouldBounce = false;
						fall();
					}

					/*
					else if( slope >= JUMP_SLOPE )
					{
						m_shouldBounce = true;
					}
					*/

					/*
					else
					{
						const sb::PhysicsManager::GravityDirection gravityDirection = sb::PhysicsManager::getInstance()->getGravityDirection();

						if( (gravityDirection == sb::PhysicsManager::Gravity_Up || gravityDirection == sb::PhysicsManager::Gravity_Down) && slope >= JUMP_SLOPE )
						{
							m_shouldBounce = true;
						}

						if( (gravityDirection == sb::PhysicsManager::Gravity_Left || gravityDirection == sb::PhysicsManager::Gravity_Right) && slope <= 1/JUMP_SLOPE )
						{
							m_shouldBounce = true;
						}
					}
					*/

					break;
				}

			case 'TLPT':
				{
					m_shouldBounce = false;

					if( m_playerState == kPlayer_Thrown )
					{
						fall();
					}

					if(m_playerState != kPlayer_Teleport)
					{
						m_playerState = kPlayer_Teleport;

						TeleportEntity* tlptEntity = sb::entity_cast<TeleportEntity>(entity);
						m_teleportTask = new TeleportTask(sf::seconds(1.f), this, tlptEntity);
						m_teleportTask->addTask();
					}

					sb::InputManager::getInstance()->feedOutput(0.6f,sf::seconds(.6f));

					break;
				}

			case 'ZONE':
				{
					//implement sort of an interface to keep track of which zone the player entity is in.
					m_shouldBounce = false;
					m_zoneEntityList.push_back(sb::entity_cast<ZoneEntity>(entity));
					break;
				}

			case 'DEAD':
				{
					//implement sort of an interface to keep track of which zone the player entity is in.
					m_shouldBounce = false;
					kill();
					break;
				}

			case 'GRAV':
				{
					GravityEntity* gravityEntity = sb::entity_cast<GravityEntity>(entity);

					if( m_shouldBounce && gravityEntity->getGravityDirection() != sb::PhysicsManager::getInstance()->getGravityDirection() )
					{
						m_shouldBounce = false;
					}

					break;
				}

			case 'LVLC':
				{
					m_shouldBounce = false;
					break;
				}

			case 'STAR':
				{
					m_shouldBounce = false;
					break;
				}

			case 'TLXT':
				{
					m_shouldBounce = false;
					break;
				}

			default:
			{
				break;
			}
	}
}

void PlayerEntity::processPreSolve( b2Contact* contact,const b2Fixture* target )
{
	b2WorldManifold manifold;
	contact->GetWorldManifold(&manifold);

	b2Vec2 contactPos = target->GetBody()->GetLocalPoint(m_ballBody.getBody()->GetPosition());
	b2Vec2 normal = manifold.normal;
	float slope = std::abs(normal.y/normal.x);


	sb::Entity* entity = static_cast<sb::Entity*>(target->GetBody()->GetUserData());
	ThrowEntity* throwEntity = sb::entity_cast<ThrowEntity>(entity);

	if( throwEntity &&
		contactPos.y > 0.f
		//&& slope >= JUMP_SLOPE
		//&& throwEntity->getPosition().y < getPosition().y
		/*&& m_playerState == kPlayer_Moving */ )
	{
		m_shouldBounce = false;
		//contact->SetEnabled(false);
		sf::Vector2f throwVelocity = ToVector(target->GetBody()->GetWorldVector(b2Vec2(0,1.0f))) * THROW_VELOCITY;
		shoot(throwVelocity);
	}
}

void PlayerEntity::control( void )
{
	const sb::PhysicsManager::GravityDirection gravityDirection = sb::PhysicsManager::getInstance()->getGravityDirection();

	const bool  leftInput = sb::InputManager::getInstance()->getLeftInput(),
				rightInput = sb::InputManager::getInstance()->getRightInput(),
				upInput = sb::InputManager::getInstance()->getUpInput(),
				downInput = sb::InputManager::getInstance()->getDownInput();

	b2Vec2 ballVelocity = m_ballBody.getBody()->GetLinearVelocity();
	b2Vec2 ballPosition = m_ballBody.getBody()->GetPosition();

	if( gravityDirection == sb::PhysicsManager::Gravity_Up || gravityDirection == sb::PhysicsManager::Gravity_Down )
	{
		if( leftInput && ballVelocity.x > -MAX_MOVE_VELOCITY )
		{
			m_ballBody.getBody()->ApplyLinearImpulse( b2Vec2( -MOVE_IMPULSE, 0 ), ballPosition );
		}

		if( rightInput && ballVelocity.x < MAX_MOVE_VELOCITY )
		{
			m_ballBody.getBody()->ApplyLinearImpulse( b2Vec2( MOVE_IMPULSE, 0 ), ballPosition );
		}
	}

	if( gravityDirection == sb::PhysicsManager::Gravity_Left || gravityDirection == sb::PhysicsManager::Gravity_Right )
	{
		if( downInput && ballVelocity.y > -MAX_MOVE_VELOCITY )
		{
			m_ballBody.getBody()->ApplyLinearImpulse( b2Vec2( 0, -MOVE_IMPULSE ), ballPosition );
		}

		if( upInput && ballVelocity.y < MAX_MOVE_VELOCITY )
		{
			m_ballBody.getBody()->ApplyLinearImpulse( b2Vec2( 0, MOVE_IMPULSE ), ballPosition );
		}
	}
}

void PlayerEntity::fall( void )
{
	m_playerState = kPlayer_Moving;
	m_ballBody.getBody()->SetGravityScale(6.0f);

	/*
	b2Vec2 ballVelocity = m_ballBody.getBody()->GetLinearVelocity();
	if( ballVelocity.y > MAX_VERTI_VELOCITY )
	{
		ballVelocity.y = MAX_VERTI_VELOCITY;
		m_ballBody.getBody()->SetLinearVelocity( ballVelocity );
	}
	*/
	m_ballBody.getBody()->SetAngularVelocity(0.f);
	m_ballBody.getBody()->SetLinearVelocity(b2Vec2(0.f,0.f));
}

void PlayerEntity::shoot( const sf::Vector2f& velocity )
{
	m_playerState = kPlayer_Thrown;
	m_ballBody.getBody()->SetGravityScale(0.0f);
	m_ballBody.getBody()->SetLinearVelocity(ToVector(velocity));

	limitHorizontalVelocity();

	sb::InputManager::getInstance()->feedOutput(0.5f, sf::seconds(0.2f));
}

void PlayerEntity::limitHorizontalVelocity()
{
	b2Vec2 ballVelocity = m_ballBody.getBody()->GetLinearVelocity();

	if( std::abs( ballVelocity.x ) > MAX_MOVE_VELOCITY )
	{
		ballVelocity.x = sb::signum<float>(ballVelocity.x) * MAX_MOVE_VELOCITY;
		m_ballBody.getBody()->SetLinearVelocity( ballVelocity );
	}
}

void PlayerEntity::limitVerticalVelocity()
{
	b2Vec2 ballVelocity = m_ballBody.getBody()->GetLinearVelocity();

	//Should this only limit when the velocity is bigger than MAX_VERTI_VELOCITY when going up?
	if( std::abs( ballVelocity.y ) > MAX_MOVE_VELOCITY )
	{
		ballVelocity.y = sb::signum<float>(ballVelocity.y) * MAX_MOVE_VELOCITY;
		m_ballBody.getBody()->SetLinearVelocity( ballVelocity );
	}
}

void PlayerEntity::bounce( void )
{
	m_shouldBounce = false;

	/*
	sb::PhysicsManager::GravityDirection

	b2Vec2 ballVelocity = m_ballBody.getBody()->GetLinearVelocity();
	b2Vec2 ballPosition = m_ballBody.getBody()->GetPosition();

	switch(gravityType)
	{
	case Gravity_Vertical:
		{
			break;
		}

	case Gravity_Horizontal:
		{

			break;
		}
	}
	*/

	const sb::PhysicsManager::GravityDirection gravityDirection = sb::PhysicsManager::getInstance()->getGravityDirection();

	b2Vec2 ballVelocity = m_ballBody.getBody()->GetLinearVelocity();
	b2Vec2 ballPosition = m_ballBody.getBody()->GetPosition();
	b2Vec2 ballImpulse = b2Vec2(0.f,0.f);


	switch (gravityDirection)
	{
	case sb::PhysicsManager::Gravity_Down:
		{
			ballVelocity.y = 0;
			ballPosition += b2Vec2(0.f,.1f);
			ballImpulse.y = JUMP_IMPULSE;
			break;
		}

	case sb::PhysicsManager::Gravity_Up:
		{
			ballVelocity.y = 0;
			ballPosition += b2Vec2(0.f,-.1f);
			ballImpulse.y = -JUMP_IMPULSE;
			break;
		}

	case sb::PhysicsManager::Gravity_Left:
		{
			ballVelocity.x = 0;
			ballPosition += b2Vec2(.1f,0.f);
			ballImpulse.x = JUMP_IMPULSE;
			break;
		}

	case sb::PhysicsManager::Gravity_Right:
		{
			ballVelocity.x = 0;
			ballPosition += b2Vec2(-.1f,0.f);
			ballImpulse.x = -JUMP_IMPULSE;
			break;
		}
	}

	m_ballBody.getBody()->SetLinearVelocity( ballVelocity );
	m_ballBody.getBody()->SetTransform(ballPosition, m_ballBody.getBody()->GetAngle());
	m_ballBody.getBody()->ApplyLinearImpulse(ballImpulse, m_ballBody.getBody()->GetPosition());

	m_bounceSound.play();

	sb::InputManager::getInstance()->feedOutput(0.3f, sf::seconds(0.15f));
}

void PlayerEntity::updatePlayerState( void )
{
	if(m_moonDead)
	{
		return;
	}

	switch(m_playerState)
	{
	case kPlayer_Moving:
		{
			control();

			if(m_shouldBounce)
			{
				bounce();
			}

			limitGravitationalVelocity();

			break;
		}

	case kPlayer_Thrown:
		{
			bool shouldFall = false;
			sb::PhysicsManager::GravityDirection gravDir = sb::PhysicsManager::getInstance()->getGravityDirection();

			if(gravDir == sb::PhysicsManager::Gravity_Down || gravDir == sb::PhysicsManager::Gravity_Up)
			{
				shouldFall = (sb::InputManager::getInstance()->getDownInput() || sb::InputManager::getInstance()->getUpInput());
			}
			else
			{
				shouldFall = (sb::InputManager::getInstance()->getRightInput() || sb::InputManager::getInstance()->getLeftInput());
			}


			if( shouldFall )
			{
				fall();
			}
			else
			{
				limitVerticalVelocity();
				limitHorizontalVelocity();
			}

			//limitGravitationalVelocity();

			break;
		}
	}
}

void PlayerEntity::kill()
{
	if(!m_moonDead)
	{
		m_moonDead = true;

		if(m_teleportTask)
		{
			m_teleportTask->removeTask();
			m_teleportTask = nullptr;
		}

		sb::EventManager::getInstance()->abortEvent(Event_NextLevel,true);

		sb::EventData* gameOverEvent = new sb::EventData( Event_GameLost );
		gameOverEvent->triggerEvent();

		sb::EventData* eventData = new sb::EventData( Event_RestartLevel );
		eventData->queueEvent(sf::seconds(0.5f));
	}	
}

void PlayerEntity::limitGravitationalVelocity( void )
{
	sb::PhysicsManager::GravityDirection gravityDirection = sb::PhysicsManager::getInstance()->getGravityDirection();

	if( gravityDirection == sb::PhysicsManager::Gravity_Down || gravityDirection == sb::PhysicsManager::Gravity_Up )
	{
		limitHorizontalVelocity();
	}

	if( gravityDirection == sb::PhysicsManager::Gravity_Left || gravityDirection == sb::PhysicsManager::Gravity_Right )
	{
		limitVerticalVelocity();
	}
}