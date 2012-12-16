#ifndef HULLENTITY_HPP
#define HULLENTITY_HPP

#include "../Entity/Entity.hpp"
#include "../Entity/Component/BodyWrapper.hpp"

class HullEntity : public Entity
{
    DEFINE_ENTITY( HullEntity, Entity, 'HULL')

public:
    virtual void Initialize( const TiXmlElement *propertyElement = NULL );

private:
	BodyWrapper _hullBody;
};

#endif
