#include "DeadZoneEntity.hpp"
#include "../Physics/PhysicsManager.hpp"
#include "../Event/ContactEventData.hpp"

REGISTER_ENTITY(DeadZoneEntity,"DeadZone")

DeadZoneEntity::DeadZoneEntity()
	:	BaseClass(),
		m_zoneBody(*this)
{
	addEventListenType(Event_BeginContact);
}

DeadZoneEntity::~DeadZoneEntity()
{
	removeEventListenType(Event_BeginContact);

	sb::PhysicsManager::getInstance()->removeSimulatable(&m_zoneBody);
}

void DeadZoneEntity::initializeEntity( const TiXmlElement *propertyElement /*= nullptr */ )
{
	BaseClass::initializeEntity(propertyElement);

	setPosition( getPosition() + 0.5f * sf::Vector2f(getScale().x,-getScale().y) * UNRATIO );

	if( propertyElement )
	{
		{
			b2BodyDef bodyDefinition;
			bodyDefinition.userData = (Entity*)this;
			bodyDefinition.position = b2Vec2(getPosition().x, getPosition().y);
			bodyDefinition.angle = 0.0f;
			bodyDefinition.fixedRotation = true;
			bodyDefinition.type = b2_staticBody;

			b2Body* zoneBody = sb::PhysicsManager::getInstance()->getPhysicsWorld()->CreateBody(&bodyDefinition);

			b2PolygonShape boxShape;
			boxShape.SetAsBox( 0.5f * getScale().x * UNRATIO, 0.5f * getScale().y * UNRATIO );

			b2FixtureDef fixtureDefinition;
			fixtureDefinition.shape = &boxShape;
			fixtureDefinition.isSensor = true;

			zoneBody->CreateFixture(&fixtureDefinition);

			m_zoneBody.setBody(zoneBody);

			sb::PhysicsManager::getInstance()->addSimulatable(&m_zoneBody);
		}
	}
}

bool DeadZoneEntity::handleEvent( const sb::EventData& theevent )
{
	switch (theevent.getEventType())
	{
	case Event_BeginContact:
		{
			const sb::ContactEventData& contactData = static_cast<const sb::ContactEventData&>(theevent);
			const b2Contact* contactInfo = contactData.getContact();

			const b2Fixture* target = nullptr;
			if( m_zoneBody.checkContact(contactInfo,target))
			{
				sb::Entity* entity = sb::getOwnerEntity(target);
				entity->releaseEntity();
			}

			break;
		}
	}

	return false;
}
