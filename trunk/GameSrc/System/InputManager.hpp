#ifndef INPUTMANAGER_HPP
#define INPUTMANAGER_HPP

#include <SFML/System/Time.hpp>
#include <Thor/Time/Timer.hpp>
#include "../Base/Singleton.hpp"
#include "../Event/EventListener.hpp"
#include <TinyXML/tinyxml.h>

namespace sb
{
	class InputManager : public Singleton<InputManager>, public EventListener
	{
		DEFINE_SINGLETON(InputManager)

	public:
		void setUpInput( const TiXmlElement* element );
		void update( sf::Time deltaTime );
		void feedOutput( float amount, sf::Time duration );
		bool handleEvent(const EventData& theevent);

		bool getLeftInput() const { return m_leftInput; }
		bool getRightInput() const { return m_rightInput; }
		bool getDownInput() const { return m_downInput; }
		bool getUpInput() const { return m_upInput; }
		bool getFallKeyHit() const { return m_fallInput; }
		float getAffectorRate() const { return m_affectorRate; }

	private:
		void clearVibration(void);
		void setVibratationState(void);

		enum InputType
		{
			kInput_Keyboard,
			kInput_Xbox,
			kInput_Kinect,
			kInput_LEAP
		} m_inputType;

		float m_affectorRate;
		bool  m_leftInput;
		bool  m_rightInput;
		bool  m_downInput;
		bool  m_upInput;
		bool  m_fallInput;
		bool  m_waitFrame;

		float m_vibrateAmount;
		thor::Timer m_vibrateDuration;
	};
}

#endif