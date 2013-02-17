#ifndef GameState_h__
#define GameState_h__

#include <SFML/Graphics.hpp>
#include "StatesDef.h"

namespace sb
{
	class GameState
	{
	public:
		GameState();
		virtual ~GameState();

		virtual void enter( void );
		virtual bool update( sf::Time deltaTime ) = 0;
		virtual void render( void ) = 0;
		virtual void exit( void );

	protected:
		inline bool isActive(void) const
		{
			return m_active;
		}

	private:
		bool m_active;
		sf::Time m_enterTime;
		sf::Time m_exitTime;
	};
}

#endif // GameState_h__
