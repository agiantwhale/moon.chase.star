#include <glog\logging.h>
#include <cmath>
#include "../Entity/PlayerEntity.hpp"
#include "../System/PhysicsManager.hpp"
#include "../System/ResourceManager.hpp"

template <typename T> int sgn(T val) {
    return (T(0) < val) - (val < T(0));
}

const float KILL_TIME = 1.5f;

REGISTER_ENTITY( PlayerEntity, "Ball" )

PlayerEntity::PlayerEntity() : Entity(), _ballBody(this), _ballSprite(this),
                               _shouldBounce(false),
                               _playerState(kPlayer_Bouncing),
                               _dead(false)
{
}

PlayerEntity::~PlayerEntity()
{
}

void PlayerEntity::Kill(void)
{
    _dead = true;
}

void PlayerEntity::Simulate( void )
{
    if(_dead) return;

    const float JUMP_IMPULSE = 18.0f;
    const float MAX_VELOCITY = 15.0f;
    const float MOVE_IMPULSE = 0.5f;

	b2Vec2 ballVelocity = _ballBody.GetBody()->GetLinearVelocity();
    b2Vec2 ballPosition = _ballBody.GetBody()->GetPosition();

    if( sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && ballVelocity.x > -MAX_VELOCITY )
    {
        _ballBody.GetBody()->ApplyLinearImpulse( b2Vec2( -MOVE_IMPULSE, 0 ), ballPosition );
    }

    if( sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && ballVelocity.x < MAX_VELOCITY )
    {
        _ballBody.GetBody()->ApplyLinearImpulse( b2Vec2( MOVE_IMPULSE, 0 ), ballPosition );
    }

    if( std::abs( ballVelocity.x ) > MAX_VELOCITY )
    {
        ballVelocity.x = sgn<float>(ballVelocity.x) * MAX_VELOCITY;
        _ballBody.GetBody()->SetLinearVelocity( ballVelocity );
    }

    if( _shouldBounce )
    {
        _shouldBounce = false;
        _ballBody.GetBody()->SetLinearVelocity( b2Vec2(ballVelocity.x, 0) );
        _ballBody.GetBody()->SetTransform(ballPosition + b2Vec2(0,0.1f), _ballBody.GetBody()->GetAngle());
        _ballBody.GetBody()->ApplyLinearImpulse(b2Vec2(0,JUMP_IMPULSE), _ballBody.GetBody()->GetPosition());
    }
}

void PlayerEntity::Update(float deltaTime)
{
    const bool  leftInput = sf::Keyboard::isKeyPressed(sf::Keyboard::Left),
                rightInput = sf::Keyboard::isKeyPressed(sf::Keyboard::Right);

    if( leftInput || rightInput && _playerState != kPlayer_Moving )
    {
        Fall();
    }
}

void PlayerEntity::Initialize( const TiXmlElement *propertyElement )
{
    BaseClass::Initialize(propertyElement);

    const float BALL_RADIUS = 0.5f;

    {
        TextureManager* textureMgr = TextureManager::GetInstance();
		sf::Sprite* ballSprite = new sf::Sprite(*textureMgr->GetResource("Resource/Ogmo/Entities/Ball.png"));
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
		fixtureDefinition.shape = &circle;

		_ballBody.CreateFixture( fixtureDefinition, "Ball" );

		_ballBody.SetSimulateCallback( BIND_MEM_CB( &PlayerEntity::Simulate, this ) );
		_ballBody.SetBeginConactCallback( BIND_MEM_CB( &PlayerEntity::BeginContact, this ) );

        _ballBody.ResetTransform();
    }
}

void PlayerEntity::BeginContact(b2Contact* contact, const b2Fixture* contactFixture )
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
                if( slope >= 1.75f ) //45 degrees is acceptable.
                {
                    _shouldBounce = false;
                } else if( (slope <= 0.25f || GetPosition().y < targetInterface->GetEntity()->GetPosition().y ) && _playerState == kPlayer_Thrown )
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
                else if( normal.x == 0.0f )
                {
                    _shouldBounce = true;
                }
                break;
            }

            default:
            {
                _shouldBounce = true;
                break;
            }
        }
    }
}
