#ifndef CAMERAMOVEENTITY_HPP
#define CAMERAMOVEENTITY_HPP

#include "../Entity/Entity.hpp"
#include "../Entity/Component/BodyWrapper.hpp"

class CameraMoveEntity : public Entity
{
    DEFINE_ENTITY( CameraMoveEntity, Entity, 'CMMV')

public:
    virtual void Initialize( const TiXmlElement *propertyElement = NULL );
    virtual void BeginContact(b2Contact* contact, const b2Fixture* contactFixture );

    void MoveCamera(bool finalize);
private:
    bool _activated;			//The ball collided with the CameraMoveEntity.
	bool _started;				//Started calling CameraMove Events.

    const float _travelTime;	//Amount of time that should be interpolated between the current location and the destination.
    Vec2D _destination;			//This is relative to the CameraMoveEntity location.

	BodyWrapper _triggerBody;
};

#endif
