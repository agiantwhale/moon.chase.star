#include "../System/EventManager.hpp"
#include "../Entity/CameraMoveEntity.hpp"
#include "../Event/EventsDef.h"
#include "../System/GraphicsManager.hpp"
#include "../Event/ContactEventData.h"
#include "../Task/CameraMoveTask.hpp"

REGISTER_ENTITY( CameraMoveEntity, "CameraMove")

CameraMoveEntity::CameraMoveEntity() : BaseClass(), _triggerBody(this), _travelTime(5.0f), _activated(false), _destination()
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

void CameraMoveEntity::ProcessContact(const b2Contact* contact, const b2Fixture* contactFixture )
{
    IPhysics *targetInterface = GetPhysicsInterface(contactFixture);

    if(!_activated && targetInterface && targetInterface->GetEntity()->GetEntityType() == 'BALL')
    {
        _activated = true;

		Task* cameraTask = new CameraMoveTask(_travelTime,0,_destination/_travelTime);
    }
}

bool CameraMoveEntity::HandleEvent( const EventData& theevent )
{
    switch (theevent.GetEventType())
    {
    case Event_BeginContact:
    {
        const ContactEventData& contactData = static_cast<const ContactEventData&>(theevent);
        const b2Contact* contactInfo = contactData.GetContact();

        if(contactInfo->GetFixtureA()==_triggerBody.LookUpFixture(("Trigger")))
        {
            ProcessContact(contactInfo,contactInfo->GetFixtureB());
        }

        if(contactInfo->GetFixtureB()==_triggerBody.LookUpFixture(("Trigger")))
        {
            ProcessContact(contactInfo,contactInfo->GetFixtureA());
        }

        break;
    }

    default:
        break;
    }

    return false;
}