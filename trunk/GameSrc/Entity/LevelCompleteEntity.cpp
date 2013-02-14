#include "../Entity/LevelCompleteEntity.hpp"
#include "../Entity/PlayerEntity.hpp"
#include "../Event/ContactEventData.h"
#include "../Task/LevelCompleteTask.hpp"

REGISTER_ENTITY(LevelCompleteEntity,"LevelComplete")

LevelCompleteEntity::LevelCompleteEntity() : BaseClass(),
											 _triggerBody(this),
											 _acceptArrival(false),
											 _endFollowRoute()
{
	addEventListenType(Event_StarArrived);
}

LevelCompleteEntity::~LevelCompleteEntity()
{
	removeEventListenType(Event_StarArrived);
}

void LevelCompleteEntity::initializeEntity( const TiXmlElement *propertyElement /*= NULL */ )
{
	BaseClass::initializeEntity(propertyElement);

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

			_triggerBody.CreateBody( bodyDefinition );

			b2PolygonShape boxShape;
			boxShape.SetAsBox( 0.5f * GetScale().x * UNRATIO, 0.5f * GetScale().y * UNRATIO );

			b2FixtureDef fixtureDefinition;
			fixtureDefinition.shape = &boxShape;
			fixtureDefinition.isSensor = true;

			_triggerBody.CreateFixture( fixtureDefinition, "Trigger" );

			_triggerBody.ResetTransform();
		}

		{
			float totalDistance = 0.0f;
			const TiXmlElement* pathNode = propertyElement->FirstChildElement("node");
			//_endFollowRoute.push_back(GetPosition());
			while(pathNode)
			{
				float x = 0.f, y = 0.f;
				pathNode->QueryFloatAttribute("x",&x);
				pathNode->QueryFloatAttribute("y",&y);

				Vec2D world((x - SCREENWIDTH/2) * UNRATIO, (y - SCREENHEIGHT/2) * UNRATIO * -1);

				_endFollowRoute.push_back(world);

				pathNode = pathNode->NextSiblingElement();
			}
		}
	}
}

void LevelCompleteEntity::ProcessContact( const b2Contact* contact, const b2Fixture* contactFixture )
{
	IPhysics *targetInterface = GetPhysicsInterface(contactFixture);

	if(isActive() && _acceptArrival && targetInterface && targetInterface->GetEntity()->GetEntityType() == 'BALL')
	{
		setActive(false);

		LevelCompleteTask* levelCompleteTask = new LevelCompleteTask(static_cast<PlayerEntity*>(targetInterface->GetEntity()),_endFollowRoute);
		levelCompleteTask->AddTask();
	}
}

bool LevelCompleteEntity::handleEvent( const EventData& theevent )
{
	switch (theevent.getEventType())
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

	case Event_StarArrived:
		{
			_acceptArrival = true;
			break;
		}

	default:
		break;
	}

	return false;
}