#include "../Entity/Entity.hpp"

Entity::Entity() :	ITransform(),
    _released( false ),
	_active(true),
    _initialized( false )
{
}

Entity::~Entity()
{
}

void Entity::Initialize( const TiXmlElement *propertyElement )
{
    _initialized = true;

    if(propertyElement)
    {
        //position
        {
            double xValue = GetPosition().x, yValue = GetPosition().y;
            propertyElement->Attribute( "x", &xValue );
            propertyElement->Attribute( "y", &yValue );

            Vec2D world((xValue - SCREENWIDTH/2) * UNRATIO, (yValue - SCREENHEIGHT/2) * UNRATIO * -1);
            SetPosition(world);
        }

        //size
        {
            double entityWidth = GetSize().x*RATIO, entityHeight = GetSize().y*RATIO;
            propertyElement->Attribute( "width", &entityWidth);
            propertyElement->Attribute( "height", &entityHeight );
            SetSize(Vec2D(entityWidth*UNRATIO,entityHeight*UNRATIO));
        }

        //angle
        {
            double angleValue = GetRotation();
            propertyElement->Attribute( "angle", &angleValue );
            SetRotation(angleValue);
        }
    }
}

void Entity::Update( float deltaTime )
{
}

void Entity::PostLoad(void)
{
}

bool Entity::HandleEvent(const EventData& theevent)
{
    return false;
}