#include "../System/PhysicsManager.hpp"
#include "BodyController.hpp"
#include "Conversion.hpp"
#include "Globals.h"

sb::BodyController::BodyController( sf::Transformable& transformable )
	:	Simulatable(),
		m_target(transformable),
		m_body(nullptr),
		m_previousPosition(),
		m_smoothPosition().
		m_previousRotation(0.f),
		m_smoothRotation(0.f)
{
	sb::PhysicsManager::getInstance()->addSimulatable(this);
}

sb::BodyController::~BodyController()
{
	sb::PhysicsManager::getInstance()->removeSimulatable(this);
}


void sb::BodyController::setBody( b2Body* body )
{
	m_body = body;
	m_body->SetUserData(static_cast<Simulatable*>(this));
}

b2Body* sb::BodyController::getBody()
{
	return m_body;
}

void sb::BodyController::updateTransform( void )
{
	if(!m_body) return;

	if(m_body->IsActive())
	{
		m_target.setPosition(ToVector(m_body->GetPosition()));
		m_target.SetRotation(m_smoothRotation * RADTODEG * -1.f);
	}
}

void sb::BodyController::smoothenTransform( float remainderRatio )
{
	if(!m_body) return;

	const float oneMinusRatio = 1.0f - remainderRatio;

	m_smoothPosition = remainderRatio * m_body->GetPosition() + oneMinusRatio * m_previousPosition;
	m_smoothRotation = remainderRatio * m_body->GetAngle() + oneMinusRatio * m_previousRotation;
}

void sb::BodyController::resetTransform( void )
{
	if(!m_body) return;

	m_smoothPosition = m_previousPosition = _body->GetPosition();
	m_smoothRotation = m_previousRotation = _body->GetAngle();
}

void sb::BodyController::setOwnerEntity( sb::Entity *entity )
{
	m_ownerEntity = entity;
}

sb::Entity* sb::BodyController::getOwnerEntity()
{
	return m_ownerEntity;
}
