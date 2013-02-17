#ifndef IntroState_h__
#define IntroState_h__

#include <sfTheora.h>
#include "GameState.hpp"

namespace sb
{
	class IntroState : public GameState
	{
	public:
		IntroState();
		virtual ~IntroState();

		virtual void enter(void);
		virtual bool update(sf::Time deltaTime);
		virtual void render(void);
		virtual void exit(void);

	private:
		sftheora::Video m_introVideo;
	};
}


#endif // IntroState_h__
