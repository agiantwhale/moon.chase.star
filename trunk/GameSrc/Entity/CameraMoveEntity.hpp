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

    void MoveCamera(float timePassed);
    void FinalizeCamera(void);

private:
    bool _activated;
	bool _started;

    const float _travelTime;
    Vec2D _startPosition;
    Vec2D _destination;

	BodyWrapper _triggerBody;
};

#endif
