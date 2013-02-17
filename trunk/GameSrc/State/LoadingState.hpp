#ifndef LoadingState_h__
#define LoadingState_h__

#include <Gwen/Controls/Base.h>
#include "GameState.hpp"
#include "../Event/EventListener.hpp"

namespace sb
{
	class LoadingState : public GameState, public EventListener
	{
	public:
		LoadingState();
		virtual ~LoadingState();

		virtual bool handleEvent(const sb::EventData& theevent);
		virtual void enter(void);
		virtual bool update(sf::Time deltaTime);
		virtual void render(void);
		virtual void exit(void);

	private:
		enum
		{
			Load_UNDEFINED,
			Load_New,
			Load_Restart,
			Load_Unload,
			Load_Next,
		} m_loadType;
		unsigned int m_sceneNum;
		Gwen::Controls::Base* m_screenBase;
		bool m_frameDrawn;
	};
}

#endif // LoadingState_h__
