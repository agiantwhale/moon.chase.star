#ifndef CAMERAMOVEENTITY_HPP
#define CAMERAMOVEENTITY_HPP

#include "../Entity/Entity.hpp"
#include "../Entity/Component/BodyWrapper.hpp"
#include "../Interface/IEventListener.hpp"

class CameraMoveEntity : public Entity
{
    DEFINE_ENTITY( CameraMoveEntity, Entity, 'CMMV')

public:
    virtual void Initialize( const TiXmlElement *propertyElement = NULL );
    virtual void ProcessContact(const b2Contact* contact, const b2Fixture* contactFixture );
    virtual bool HandleEvent( const EventData& theevent );
private:
    const float _travelTime;	//Amount of time that should be interpolated between the current location and the destination.
	Vec2D _destination;

    BodyWrapper _triggerBody;
};

#endif
