#ifndef LOADINGSTATE_HPP
#define LOADINGSTATE_HPP

#include <Gwen/Controls/Base.h>
#include "../Interface/IState.hpp"
#include "../Interface/IEventListener.hpp"

class LoadingState : public GameState, public EventListener
{
public:
	LoadingState();
	virtual ~LoadingState();

	virtual bool handleEvent(const EventData& theevent);
	virtual void enter(void);
	virtual bool update(float deltaTime);
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
	} _loadType;
	unsigned int _sceneNum;
	Gwen::Controls::Base* _screenBase;
	bool _frameDrawn;
};

#endif