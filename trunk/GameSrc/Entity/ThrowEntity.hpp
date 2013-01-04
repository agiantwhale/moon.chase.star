#ifndef BLOCKENTITY_HPP
#define BLOCKENTITY_HPP

#include "../Entity/Entity.hpp"
#include "../Entity/Component/BodyWrapper.hpp"
#include "../Entity/Component/SpriteWrapper.hpp"

class ThrowEntity : public Entity
{
    DEFINE_ENTITY( ThrowEntity, Entity, 'THRW')

public:
    virtual void Initialize( const TiXmlElement *propertyElement = NULL );
    virtual bool HandleEvent(const EventData& theevent);
    virtual void ProcessContact(const b2Contact* contact, const b2Fixture* contactFixture );
private:
    BodyWrapper _throwBody;
    SpriteWrapper _throwSprite;
};

#endif
