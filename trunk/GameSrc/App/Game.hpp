#ifndef GAME_H
#define GAME_H

#include <SFML/Graphics.hpp>
#include <CxxTL/tri_logger.hpp>
#include <unordered_map>
#include <Thor/Time.hpp>

#include <Gwen/Controls/Canvas.h>
#include <Gwen/Input/SFML.h>

#include "../State/GameState.hpp"
#include "../Base/Singleton.hpp"

namespace sb
{

	struct Settings
	{
		bool fullscreen;
		bool vSync;
	};

	class Game : public Singleton<Game>, public sf::RenderWindow
	{
		DEFINE_SINGLETON( Game )

	public:
		void initialize( void );
		void start( void );
		inline void quit(void)
		{
			close();
			m_isRunning = false;
		}

		inline bool isRunning( void ) const
		{
			return m_isRunning;
		}
		inline sf::Time getTime(void) const
		{
			return m_gameClock.getElapsedTime();
		}

		inline void setNextStateType(StateType val)
		{
			m_nextStateType = val;
		}

		inline void pauseFrameTimer(void)
		{
			m_frameClock.stop();
		}

		inline void resumeFrameTimer(void)
		{
			m_frameClock.start();
		}

	private:
		typedef std::unordered_map<StateType,GameState*> StateMap;

		void pollEvents(void);
		void changeStates(void);
		void update(void);
		void render(void);

		bool m_isRunning;
		bool m_isPaused;
		bool m_shouldTakeScreenshot;
		bool m_shouldSwitchState;

		StateType m_currentStateType;
		StateType m_nextStateType;
		GameState* m_currentState;
		StateMap m_stateMap;

		thor::StopWatch   m_frameClock;
		sf::Clock		  m_gameClock;
	};

}

#endif
