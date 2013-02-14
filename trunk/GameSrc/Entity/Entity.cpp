#include "../Entity/Entity.hpp"

Entity::Entity() :
	ITransform(),
    m_released( false ),
	m_active(true),
    m_initialized( false )
{
}

Entity::~Entity()
{
}

void Entity::initializeEntity( const TiXmlElement *propertyElement )
{
    m_initialized = true;

    if(propertyElement)
    {
        //position
        {
			float x = 0.f, y = 0.f;
			propertyElement->QueryFloatAttribute("x",&x);
			propertyElement->QueryFloatAttribute("y",&y);

            Vec2D world((x - SCREENWIDTH/2) * UNRATIO, (y - SCREENHEIGHT/2) * UNRATIO * -1);
            SetPosition(world);
        }

        //size
        {
			float w = 1.f, h = 1.f;
			propertyElement->QueryFloatAttribute("width",&w);
			propertyElement->QueryFloatAttribute("height",&h);

            SetScale(Vec2D(w,h));
        }

        //angle
        {
			float r = 0.f;
			propertyElement->QueryFloatAttribute("angle",&r);

            SetRotation(r);
        }
    }
}

void Entity::update( sf::Time deltaTime )
{
}

void Entity::postLoad(void)
{
}

bool Entity::handleEvent(const EventData& theevent)
{
    return false;
}