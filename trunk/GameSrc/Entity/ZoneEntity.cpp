#include "../Entity/ZoneEntity.hpp"
#include "../Base/Math.hpp"
#include "../Event/ContactEventData.h"
#include "../Task/CameraMoveTask.hpp"
#include "../System/GraphicsManager.hpp"
#include "../System/SceneManager.hpp"

std::list<Task*> ZoneEntity::_taskList;

ZoneEntity::ZoneEntity()
	:	BaseClass(),
		_zoneBody(this),
		_containsBall(false)
{
}

ZoneEntity::~ZoneEntity()
{
}

void ZoneEntity::Initialize( const TiXmlElement *propertyElement /*= NULL */ )
{
	BaseClass::Initialize(propertyElement);

	{
		b2BodyDef bodyDefinition;
		bodyDefinition.userData = (IPhysics*)this;
		bodyDefinition.position = b2Vec2(GetPosition().x, GetPosition().y);
		bodyDefinition.angle = 0.0f;
		bodyDefinition.fixedRotation = true;
		bodyDefinition.type = b2_staticBody;

		_zoneBody.CreateBody( bodyDefinition );

		b2PolygonShape boxShape;
		boxShape.SetAsBox( 0.5f * SCREENWIDTH * UNRATIO, 0.5f * SCREENHEIGHT * UNRATIO );

		b2FixtureDef fixtureDef;
		fixtureDef.isSensor = true;
		fixtureDef.shape = &boxShape;

		_zoneBody.CreateFixture( fixtureDef, "Zone" );

		_zoneBody.ResetTransform();
	}
}

void ZoneEntity::PostLoad()
{
	ITransform* moonTransform = SceneManager::GetInstance()->FindTransform("Player");

	Vec2D moonPos = moonTransform->GetPosition();
	if(_zoneBody.LookUpFixture("Zone")->TestPoint(moonPos))
	{
		for(unsigned int i = 0; i < GraphicsManager::GetInstance()->GetRenderLayerStackSize(); i++ )
		{
			GraphicsManager::GetInstance()->GetRenderLayer(i)->GetCamera().SetPosition(GetPosition());
		}
	}
}

bool ZoneEntity::HandleEvent( const EventData& theevent )
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

				if(targetInterface->GetEntity()->GetEntityType() == 'BALL')
				{
					_containsBall = true;

					for(std::list<Task*>::iterator iter = _taskList.begin(); iter != _taskList.end(); iter++ )
					{
						Task* moveTask = (*iter);
						moveTask->RemoveTask();
						delete moveTask;
					}
					_taskList.clear();

					for(unsigned int i = 0; i < GraphicsManager::GetInstance()->GetRenderLayerStackSize(); i++ )
					{
						Vec2D deltaDistance = (GetPosition()-GraphicsManager::GetInstance()->GetRenderLayer(i)->GetCamera().GetPosition());
						float affector = 1.0f;

						deltaDistance.x = signum<float>(deltaDistance.x) * SCREENWIDTH * UNRATIO;
						deltaDistance.y = signum<float>(deltaDistance.y) * SCREENHEIGHT * UNRATIO;

						if(i==0)
						{
							affector = 0.5f;
						}

						CameraMoveTask* cameraTask = new CameraMoveTask(deltaDistance * affector,i,affector);
						cameraTask->AddTask();
						_taskList.push_back(cameraTask);
					}
				}
			}

			break;
		}

	case Event_EndContact:
		{
			const ContactEventData& contactData = static_cast<const ContactEventData&>(theevent);
			const b2Contact* contactInfo = contactData.GetContact();

			const b2Fixture* target = nullptr;
			if(_zoneBody.IsContactRelated(contactInfo,target))
			{
				IPhysics *targetInterface = GetPhysicsInterface(target);

				if(targetInterface->GetEntity()->GetEntityType() == 'BALL')
				{
					_containsBall = false;
					Release();
				}
			}

			break;
		}
	}

	return false;
}
