#include "../Entity/LevelCompleteEntity.hpp"
#include "../Event/ContactEventData.h"
#include "../Event/NextLevelEventData.hpp"

REGISTER_ENTITY(LevelCompleteEntity,"LevelComplete")

LevelCompleteEntity::LevelCompleteEntity() : BaseClass(),
											 _triggerBody(this)
{

}

LevelCompleteEntity::~LevelCompleteEntity()
{

}

void LevelCompleteEntity::Initialize( const TiXmlElement *propertyElement /*= NULL */ )
{
	BaseClass::Initialize(propertyElement);

	SetPosition( GetPosition() + 0.5f * Vec2D(GetScale().x,-GetScale().y) * UNRATIO );

	if( propertyElement )
	{
		{
			_nextMapFile = propertyElement->Attribute("NextMapFile");
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

void LevelCompleteEntity::ProcessContact( const b2Contact* contact, const b2Fixture* contactFixture )
{
	IPhysics *targetInterface = GetPhysicsInterface(contactFixture);

	if(IsActive() && targetInterface && targetInterface->GetEntity()->GetEntityType() == 'BALL')
	{
		SetActive(false);

		NextLevelEventData* eventData = new NextLevelEventData(_nextMapFile);
		eventData->QueueEvent(0.5f);
	}
}

bool LevelCompleteEntity::HandleEvent( const EventData& theevent )
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

