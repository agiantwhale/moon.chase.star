#include "HullEntity.hpp"
#include "../Physics/PhysicsManager.hpp"

REGISTER_ENTITY( HullEntity, "Hull" )

HullEntity::HullEntity() : BaseClass(), m_hullBody(*this)
{

}

HullEntity::~HullEntity()
{

}


void HullEntity::initializeEntity( const TiXmlElement *propertyElement /* = NULL */ )
{
	assert( propertyElement != nullptr ); //Property element can't be nullptr for hull entity, we MUST read data off from it!

	BaseClass::initializeEntity(propertyElement);

	if(propertyElement)
	{
		double sizeX = 0.0f, sizeY = 0.0f;
		propertyElement->Attribute("w",&sizeX );
		propertyElement->Attribute("h",&sizeY );
		const float hullX = sizeX * UNRATIO;
		const float hullY = sizeY * UNRATIO;

		setPosition( getPosition() + sf::Vector2f( hullX*0.5f,hullY*-0.5f ) );

		{
			b2BodyDef bodyDefinition;
			bodyDefinition.userData = (Entity*)this;
			bodyDefinition.position = b2Vec2(getPosition().x, getPosition().y);
			bodyDefinition.angle = 0.0f;
			bodyDefinition.fixedRotation = true;
			bodyDefinition.type = b2_staticBody;

			b2Body* hullBody = sb::PhysicsManager::getInstance()->getPhysicsWorld()->CreateBody(&bodyDefinition);

			b2PolygonShape boxShape;
			boxShape.SetAsBox( 0.5f * hullX, 0.5f * hullY );

			b2FixtureDef fixtureDef;
			fixtureDef.density = 1.0f;
			fixtureDef.restitution = 0.5f;
			fixtureDef.friction = 0.0f;
			fixtureDef.shape = &boxShape;

			hullBody->CreateFixture(&fixtureDef);

			m_hullBody.setBody(hullBody);
		}
	}
}