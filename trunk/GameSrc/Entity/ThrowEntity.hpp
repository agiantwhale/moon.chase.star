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
private:
    BodyWrapper _throwBody;
    SpriteWrapper _throwSprite;
};

#endif
