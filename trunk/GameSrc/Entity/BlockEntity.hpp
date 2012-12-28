#ifndef BLOCKENTITY_HPP
#define BLOCKENTITY_HPP

#include "../Entity/Entity.hpp"
#include "../Entity/Component/SpriteWrapper.hpp"
#include "../Entity/Component/BodyWrapper.hpp"

class BlockEntity : public Entity
{
    DEFINE_ENTITY( BlockEntity, Entity, 'BLCK')

public:
    virtual void Initialize( const TiXmlElement *propertyElement = NULL );
    virtual void ProcessContact(const b2Contact* contact, const b2Fixture* contactFixture );
    virtual bool HandleEvent(const EventData& theevent);
    virtual void Simulate(void);

private:
    bool _shouldFall;

    BodyWrapper _blockBody;
    SpriteWrapper _blockSprite;
};

#endif
