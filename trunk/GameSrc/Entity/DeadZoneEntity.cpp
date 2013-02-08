#include "../Entity/DeadZoneEntity.hpp"
#include "../Event/ContactEventData.h"

REGISTER_ENTITY(DeadZoneEntity,"DeadZone")

DeadZoneEntity::DeadZoneEntity()
	:	BaseClass(),
		_zoneBody(this)
{

}

DeadZoneEntity::~DeadZoneEntity()
{

}

void DeadZoneEntity::Initialize( const TiXmlElement *propertyElement /*= nullptr */ )
{
	BaseClass::Initialize(propertyElement);

	SetPosition( GetPosition() + 0.5f * Vec2D(GetScale().x,-GetScale().y) * UNRATIO );

	if( propertyElement )
	{
		{
			b2BodyDef bodyDefinition;
			bodyDefinition.userData = (IPhysics*)this;
			bodyDefinition.position = b2Vec2(GetPosition().x, GetPosition().y);
			bodyDefinition.angle = 0.0f;
			bodyDefinition.fixedRotation = true;
			bodyDefinition.type = b2_staticBody;

			_zoneBody.CreateBody( bodyDefinition );

			b2PolygonShape boxShape;
			boxShape.SetAsBox( 0.5f * GetScale().x * UNRATIO, 0.5f * GetScale().y * UNRATIO );

			b2FixtureDef fixtureDefinition;
			fixtureDefinition.shape = &boxShape;
			fixtureDefinition.isSensor = true;

			_zoneBody.CreateFixture( fixtureDefinition, "Trigger" );

			_zoneBody.ResetTransform();
		}
	}
}

bool DeadZoneEntity::HandleEvent( const EventData& theevent )
{
	switch (theevent.GetEventType())
	{
	case Event_BeginContact:
		{
			const ContactEventData& contactData = static_cast<const ContactEventData&>(theevent);
			const b2Contact* contactInfo = contactData.GetContact();

			const b2Fixture* target = nullptr;
			if(_zoneBody.IsContactRelated(contactInfo,target))
			{
				IPhysics *targetInterface = GetPhysicsInterface(target);

				if(targetInterface->GetEntity()->GetEntityType() != 'BALL')
				{
					targetInterface->GetEntity()->Release();
				}
			}

			break;
		}
	}

	return false;
}
