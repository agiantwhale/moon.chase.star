#include <glog\logging.h>
#include "../Entity/CameraMoveEntity.hpp"
#include "../Event/CameraMoveEventData.hpp"

REGISTER_ENTITY( CameraMoveEntity, "CameraMove")

CameraMoveEntity::CameraMoveEntity() : BaseClass(), _triggerBody(this), _travelTime(5.0f), _destination(0.0f,0.0f), _startPosition(0.0f,0.0f), _activated(false), _started(false)
{
}

CameraMoveEntity::~CameraMoveEntity()
{
}

void CameraMoveEntity::Initialize( const TiXmlElement *propertyElement )
{
    BaseClass::Initialize(propertyElement);

	SetPosition( GetPosition() + 0.5f * sf::Vector2f(GetScale().x,-GetScale().y) );

    if( propertyElement )
    {
        {
            string moveDir = propertyElement->Attribute("MoveDir");
            if( moveDir == "Top" )
            {
                _destination = sf::Vector2f(0,SCREENHEIGHT*UNRATIO);
            }

            if( moveDir == "Down" )
            {
                _destination = sf::Vector2f(0,-SCREENHEIGHT*UNRATIO);
            }

            if( moveDir == "Left" )
            {
                _destination = sf::Vector2f(-SCREENWIDTH*UNRATIO,0);
            }

            if( moveDir == "Right" )
            {
                _destination = sf::Vector2f(SCREENWIDTH*UNRATIO,0);
            }
        }

        {
            b2BodyDef bodyDefinition;
            bodyDefinition.userData = (IPhysics*)this;
            bodyDefinition.position = b2Vec2(GetPosition().x, GetPosition().y);
            bodyDefinition.angle = 0.0f;
            bodyDefinition.fixedRotation = true;
            bodyDefinition.type = b2_staticBody;

			_triggerBody.CreateBody( bodyDefinition );

            b2PolygonShape boxShape;
			boxShape.SetAsBox( 0.5f * GetScale().x, 0.5f * GetScale().y );

            b2FixtureDef fixtureDefinition;
            fixtureDefinition.shape = &boxShape;
            fixtureDefinition.isSensor = true;

			_triggerBody.CreateFixture( fixtureDefinition, "Trigger" );

			_triggerBody.ResetTransform();
        }
    }
}

void CameraMoveEntity::BeginContact(b2Contact* contact, const b2Fixture* contactFixture )
{
    IPhysics *targetInterface = GetPhysicsInterface(contactFixture);

    if(!_activated && targetInterface && targetInterface->GetEntity()->GetEntityType() == 'BALL')
    {
        _activated = true;
		_started = true;

		CameraMoveEventData* eventData = new CameraMoveEventData( _travelTime, _destination, false );
		EventManager::GetInstance()->QueueEvent( eventData );
    }
}