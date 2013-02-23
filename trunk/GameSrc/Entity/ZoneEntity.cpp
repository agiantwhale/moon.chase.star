#include "ZoneEntity.hpp"
#include "PlayerEntity.hpp"
#include "StarEntity.hpp"
#include "../Helper/Conversion.hpp"
#include "../Base/Math.hpp"
#include "../Event/ContactEventData.hpp"
#include "../Task/CameraMoveTask.hpp"
#include "../Physics/PhysicsManager.hpp"
#include "../System/GraphicsManager.hpp"
#include "../System/RenderLayer.hpp"
#include "../System/SceneManager.hpp"

std::list<sb::Task*> ZoneEntity::m_taskList;

ZoneEntity::ZoneEntity()
	:	BaseClass(),
		m_zoneBody(*this),
		m_containsBall(false)
{
	addEventListenType(Event_BeginContact);
	addEventListenType(Event_EndContact);
}

ZoneEntity::~ZoneEntity()
{
	removeEventListenType(Event_BeginContact);
	removeEventListenType(Event_EndContact);

	sb::PhysicsManager::getInstance()->removeSimulatable(&m_zoneBody);
}

void ZoneEntity::initializeEntity( const TiXmlElement *propertyElement /*= NULL */ )
{
	BaseClass::initializeEntity(propertyElement);

	{
		b2BodyDef bodyDefinition;
		bodyDefinition.userData = (Entity*)this;
		bodyDefinition.position = ToVector(getPosition());
		bodyDefinition.angle = 0.0f;
		bodyDefinition.fixedRotation = true;
		bodyDefinition.type = b2_staticBody;

		b2Body* zoneBody = sb::PhysicsManager::getInstance()->getPhysicsWorld()->CreateBody(&bodyDefinition);

		b2PolygonShape boxShape;
		boxShape.SetAsBox( 0.5f * SCREENWIDTH * UNRATIO, 0.5f * SCREENHEIGHT * UNRATIO );

		b2FixtureDef fixtureDef;
		fixtureDef.isSensor = true;
		fixtureDef.shape = &boxShape;

		zoneBody->CreateFixture(&fixtureDef);

		m_zoneBody.setBody(zoneBody);

		sb::PhysicsManager::getInstance()->addSimulatable(&m_zoneBody);
	}
}

void ZoneEntity::postLoad()
{
	sf::Vector2f moonPos = sb::SceneManager::getInstance()->getPlayerEntity()->getPosition();

	if(m_zoneBody.getBody()->GetFixtureList()->TestPoint(ToVector(moonPos)))
	{
		for(unsigned int i = 0; i < sb::GraphicsManager::getInstance()->getRenderLayerStackSize(); i++ )
		{
			sb::GraphicsManager::getInstance()->getRenderLayer(i)->getCamera().setPosition(getPosition());
		}
	}
}

bool ZoneEntity::handleEvent( const sb::EventData& theevent )
{
	switch (theevent.getEventType())
	{
	case Event_BeginContact:
		{
			const sb::ContactEventData& contactData = static_cast<const sb::ContactEventData&>(theevent);
			const b2Contact* contactInfo = contactData.getContact();

			const b2Fixture* target = nullptr;

			if(m_zoneBody.checkContact(contactInfo,target))
			{
				sb::Entity* entity = static_cast<sb::Entity*>(target->GetBody()->GetUserData());
				PlayerEntity* playerEntity = sb::entity_cast<PlayerEntity>(entity);

				if(playerEntity)
				{
					m_containsBall = true;
				}

				StarEntity* starEntity = sb::entity_cast<StarEntity>(entity);
				if(starEntity)
				{
					for(std::list<sb::Task*>::iterator iter = m_taskList.begin(); iter != m_taskList.end(); iter++ )
					{
						sb::Task* moveTask = (*iter);
						moveTask->removeTask();
						delete moveTask;
					}
					m_taskList.clear();

					for(unsigned int i = 0; i < sb::GraphicsManager::getInstance()->getRenderLayerStackSize(); i++ )
					{
						sf::Vector2f deltaDistance = (getPosition()-sb::GraphicsManager::getInstance()->getRenderLayer(i)->getCamera().getPosition());
						float affector = 1.0f;

						deltaDistance.x = sb::signum<float>(deltaDistance.x) * SCREENWIDTH * UNRATIO;
						deltaDistance.y = sb::signum<float>(deltaDistance.y) * SCREENHEIGHT * UNRATIO;

						if(i==0)
						{
							affector = 0.5f;
						}

						CameraMoveTask* cameraTask = new CameraMoveTask(deltaDistance * affector,i,affector);
						cameraTask->addTask();
						m_taskList.push_back(cameraTask);
					}
				}
			}

			break;
		}

	case Event_EndContact:
		{
			const sb::ContactEventData& contactData = static_cast<const sb::ContactEventData&>(theevent);
			const b2Contact* contactInfo = contactData.getContact();

			const b2Fixture* target = nullptr;
			if(m_zoneBody.checkContact(contactInfo,target))
			{
				sb::Entity* entity = sb::getOwnerEntity(target);
				PlayerEntity* playerEntity = sb::entity_cast<PlayerEntity>(entity);

				if(playerEntity)
				{
					m_containsBall = false;
					releaseEntity();
				}
			}
			
			break;
		}
	}

	return false;
}
