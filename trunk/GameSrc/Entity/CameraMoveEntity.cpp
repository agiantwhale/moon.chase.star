#include "../System/EventManager.hpp"
#include "../Entity/CameraMoveEntity.hpp"
#include "../Event/EventsDef.h"
#include "../System/GraphicsManager.hpp"
#include "../Event/ContactEventData.h"
#include "../Task/CameraMoveTask.hpp"

REGISTER_ENTITY( CameraMoveEntity, "CameraMove")

CameraMoveEntity::CameraMoveEntity() : BaseClass(), _triggerBody(this), _travelTime(2.5f), _destination()
{

}

CameraMoveEntity::~CameraMoveEntity()
{
}

void CameraMoveEntity::Initialize( const TiXmlElement *propertyElement )
{
    BaseClass::Initialize(propertyElement);

    SetPosition( GetPosition() + 0.5f * Vec2D(GetScale().x,-GetScale().y) * UNRATIO );

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
            boxShape.SetAsBox( 0.5f * GetScale().x * UNRATIO, 0.5f * GetScale().y * UNRATIO );

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

    if(IsActive() && targetInterface && targetInterface->GetEntity()->GetEntityType() == 'STAR')
    {
        SetActive(false);

		for(int i = 0; i < GraphicsManager::GetInstance()->GetRenderLayerStackSize(); i++ )
		{
			Task* cameraTask = new CameraMoveTask(_travelTime,i,_destination/_travelTime);
			cameraTask->AddTask();
		}
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

		const b2Fixture* target = nullptr;
		if(_triggerBody.IsContactRelated(contactInfo,target))
		{
			ProcessContact(contactInfo,target);
		}

        break;
    }

    default:
        break;
    }

    return false;
}