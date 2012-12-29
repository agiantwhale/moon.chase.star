#include "../Entity/HullEntity.hpp"
#include <CxxTL/tri_logger.hpp>

REGISTER_ENTITY( HullEntity, "Hull")

HullEntity::HullEntity() : BaseClass(), _hullBody(this)
{
}

HullEntity::~HullEntity()
{
}

void HullEntity::Initialize( const TiXmlElement *propertyElement )
{
    BaseClass::Initialize(propertyElement);

    if( propertyElement )
    {
        double sizeX = 0.0f, sizeY = 0.0f;
        propertyElement->Attribute("w",&sizeX );
        propertyElement->Attribute("h",&sizeY );
        const float hullX = sizeX * UNRATIO;
        const float hullY = sizeY * UNRATIO;

        SetPosition( GetPosition() + Vec2D(hullX*0.5f,hullY*-0.5f) );

        {
            b2BodyDef bodyDefinition;
            bodyDefinition.userData = (IPhysics*)this;
            bodyDefinition.position = b2Vec2(GetPosition().x, GetPosition().y);
            bodyDefinition.angle = 0.0f;
            bodyDefinition.fixedRotation = true;
            bodyDefinition.type = b2_staticBody;

            _hullBody.CreateBody( bodyDefinition );

            b2PolygonShape boxShape;
            boxShape.SetAsBox( 0.5f * hullX, 0.5f * hullY );

            b2FixtureDef fixtureDef;
            fixtureDef.density = 1.0f;
            fixtureDef.restitution = 0.0f;
            fixtureDef.friction = 0.0f;
            fixtureDef.shape = &boxShape;

            _hullBody.CreateFixture( fixtureDef, "Hull" );

            _hullBody.ResetTransform();
        }
    }
    else
    {
        TRI_LOG_STR("Wrong hull entity initialization method.");
    }
}
