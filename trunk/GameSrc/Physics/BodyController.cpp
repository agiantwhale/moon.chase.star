#include "../Base/Globals.hpp"
#include "BodyController.hpp"

namespace sb
{
	BodyController::BodyController( sf::Transformable& transformable )
		:	Simulatable(),
			m_target(transformable),
			m_body(nullptr),
			m_previousPosition(),
			m_smoothPosition(),
			m_previousRotation(0.f),
			m_smoothRotation(0.f)
	{

	}

	BodyController::~BodyController()
	{

	}

	void BodyController::updateTransform( void )
	{
		assert(m_body != nullptr);
		if(m_body) return;

		if(m_body && m_body->IsActive())
		{
			m_target.setPosition(sf::Vector2f(m_smoothPosition.x,m_smoothPosition.y));
			m_target.setRotation(-1.f * m_smoothRotation * RADTODEG);
		}
	}

	void BodyController::smoothenTransform( float remainderRatio )
	{
		assert(m_body != nullptr);
		if(m_body) return;

		const float oneMinusRatio = 1.0f - remainderRatio;

		m_smoothPosition = remainderRatio * m_body->GetPosition() + oneMinusRatio * m_previousPosition;
		m_smoothRotation = remainderRatio * m_body->GetAngle() + oneMinusRatio * m_previousRotation;
	}

	void BodyController::resetTransform( void )
	{
		assert(m_body != nullptr);
		if(m_body) return;

		m_smoothPosition = m_previousPosition = m_body->GetPosition();
		m_smoothRotation = m_previousRotation = m_body->GetAngle();
	}

}