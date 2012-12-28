#ifndef GAME_H
#define GAME_H

#include <SFML/Graphics.hpp>
#include <glog\logging.h>
#include <unordered_map>

#include <Gwen/Controls/Canvas.h>
#include <Gwen/Input/SFML.h>

#include "../Interface/IState.hpp"
#include "../Interface/IEventListner.hpp"
#include "../Base/Singleton.hpp"

class Game : public Singleton<Game>, public sf::RenderWindow, public IEventListener
{
    DEFINE_SINGLETON( Game )

public:
    virtual bool HandleEvent(const EventData& theevent);

    void Initialize( void );
    void Start( void );
    inline void Quit(void)
    {
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

private:
    typedef std::unordered_map<StateType,IState*> StateMap;

    void PollEvents(void);
    void ChangeStates(void);
    void Update(void);
    void Render(void);

    bool _isRunning;
    bool _shouldSwitchState;

    StateType _currentStateType;
    StateType _nextStateType;
    IState* _currentState;
    StateMap _stateMap;

    sf::Clock         _frameClock;
    sf::Clock		  _gameClock;
};

#endif
