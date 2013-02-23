#include "LevelCompleteEntity.hpp"
#include "PlayerEntity.hpp"
#include "../Physics/PhysicsManager.hpp"
#include "../Event/ContactEventData.hpp"
#include "../Task/LevelCompleteTask.hpp"

REGISTER_ENTITY(LevelCompleteEntity,"LevelComplete")

LevelCompleteEntity::LevelCompleteEntity() : BaseClass(),
											 m_triggerBody(*this),
											 m_acceptArrival(false),
											 m_endFollowRoute()
{
	addEventListenType(Event_BeginContact);
	addEventListenType(Event_StarArrived);
}

LevelCompleteEntity::~LevelCompleteEntity()
{
	removeEventListenType(Event_BeginContact);
	removeEventListenType(Event_StarArrived);

	sb::PhysicsManager::getInstance()->removeSimulatable(&m_triggerBody);
}

void LevelCompleteEntity::initializeEntity( const TiXmlElement *propertyElement /*= NULL */ )
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

			b2Body* triggerBody = sb::PhysicsManager::getInstance()->getPhysicsWorld()->CreateBody(&bodyDefinition);

			b2PolygonShape boxShape;
			boxShape.SetAsBox( 0.5f * getScale().x * UNRATIO, 0.5f * getScale().y * UNRATIO );

			b2FixtureDef fixtureDefinition;
			fixtureDefinition.shape = &boxShape;
			fixtureDefinition.isSensor = true;

			triggerBody->CreateFixture(&fixtureDefinition);
			
			m_triggerBody.setBody(triggerBody);

			sb::PhysicsManager::getInstance()->addSimulatable(&m_triggerBody);
		}

		{
			float totalDistance = 0.0f;
			for(const TiXmlElement* pathNode = propertyElement->FirstChildElement("node"); pathNode != nullptr; pathNode = pathNode->NextSiblingElement())
			{
				float x = 0.f, y = 0.f;
				pathNode->QueryFloatAttribute("x",&x);
				pathNode->QueryFloatAttribute("y",&y);

				sf::Vector2f world((x - SCREENWIDTH/2) * UNRATIO, (y - SCREENHEIGHT/2) * UNRATIO * -1);

				m_endFollowRoute.push_back(world);
			}
		}
	}
}

void LevelCompleteEntity::processContact( const b2Contact* contact, const b2Fixture* contactFixture )
{
	Entity* targetEntity = static_cast<Entity*>(contactFixture->GetBody()->GetUserData());
	PlayerEntity* playerEntity = sb::entity_cast<PlayerEntity>(targetEntity);

	if(isActive() && m_acceptArrival && playerEntity)
	{
		setActive(false);

		sb::EventData* winEvent = new sb::EventData(Event_GameWon);
		winEvent->triggerEvent();

		LevelCompleteTask* levelCompleteTask = new LevelCompleteTask(sb::entity_cast<PlayerEntity>(targetEntity),m_endFollowRoute);
		levelCompleteTask->addTask();
	}
}

bool LevelCompleteEntity::handleEvent( const sb::EventData& theevent )
{
	switch (theevent.getEventType())
	{
	case Event_BeginContact:
		{
			const sb::ContactEventData& contactData = static_cast<const sb::ContactEventData&>(theevent);
			const b2Contact* contactInfo = contactData.getContact();

			const b2Fixture* target = nullptr;
			if(m_triggerBody.checkContact(contactInfo,target))
			{
				processContact(contactInfo, target);
			}

			break;
		}

	case Event_StarArrived:
		{
			m_acceptArrival = true;
			break;
		}

	default:
		break;
	}

	return false;
}