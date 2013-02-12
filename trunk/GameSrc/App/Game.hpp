#ifndef GAME_H
#define GAME_H

#include <SFML/Graphics.hpp>
#include <CxxTL/tri_logger.hpp>
#include <unordered_map>
#include <Thor/Time.hpp>

#include <Gwen/Controls/Canvas.h>
#include <Gwen/Input/SFML.h>

#include "../Interface/IState.hpp"
#include "../Base/Singleton.hpp"

struct Settings
{
	bool fullscreen;
	bool vSync;
};

class Game : public Singleton<Game>, public sf::RenderWindow
{
    DEFINE_SINGLETON( Game )

public:
    void Initialize( void );
    void Start( void );
    inline void Quit(void)
    {
		close();
        _isRunning = false;
    }

    inline bool IsRunning( void ) const
    {
        return _isRunning;
    }
    inline float GetTime(void) const
    {
        return _gameClock.getElapsedTime().asSeconds();
    }

    inline void SetNextStateType(StateType val)
    {
        _nextStateType = val;
    }

	inline void PauseFrameTimer(void)
	{
		_frameClock.stop();
	}

	inline void ResumeFrameTimer(void)
	{
		_frameClock.start();
	}

private:
    typedef std::unordered_map<StateType,IState*> StateMap;

    void PollEvents(void);
    void ChangeStates(void);
    void Update(void);
    void Render(void);

    bool _isRunning;
	bool _isPaused;
	bool _shouldTakeScreenshot;
    bool _shouldSwitchState;

    StateType _currentStateType;
    StateType _nextStateType;
    IState* _currentState;
    StateMap _stateMap;

    thor::StopWatch   _frameClock;
    sf::Clock		  _gameClock;
};

#endif
