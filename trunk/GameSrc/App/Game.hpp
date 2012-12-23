#ifndef GAME_H
#define GAME_H

#include <SFML/Graphics.hpp>
#include <glog\logging.h>

#include "../Interface/IState.hpp"
#include "../Base/Singleton.hpp"

class Game : public Singleton<Game>, public sf::RenderWindow
{
	DEFINE_SINGLETON( Game )

public:
	void Initialize( void );
	void Start( void );
	inline void Quit(void)
	{
	    _isRunning = false;
	}

	inline bool IsRunning( void ) const { return _isRunning; }
	inline float GetTime(void) const { return _gameClock.getElapsedTime().asSeconds(); }

private:
    bool HandleEvents(void);
    void Update(void);
	void Render(void);

	bool _isRunning;
	sf::Clock         _frameClock;
	sf::Clock		  _gameClock;
};

#endif
