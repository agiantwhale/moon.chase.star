#ifndef GUIMANAGER_HPP
#define GUIMANAGER_HPP

#include "../Base/Singleton.hpp"
#include "../Interface/IEventListener.hpp"

#include <SFML/Graphics.hpp>
#include <Gwen/Gwen.h>
#include <Gwen/Skin.h>
#include <Gwen/Input/SFML.h>

class GUIManager : public Singleton<GUIManager>, public IEventListener
{
    DEFINE_SINGLETON(GUIManager)

public:
	virtual bool HandleEvent(const EventData& theevent);
    void SetUpGUI(void);
	void Render(void);

	Gwen::Controls::Canvas* GetCanvas() const { return _gwenCanvas; }

private:
    Gwen::Renderer::Base*		_gwenRenderer;
    Gwen::Controls::Canvas*		_gwenCanvas;
	Gwen::Skin::Base*			_gwenSkin;
    Gwen::Input::SFML*			_gwenInput;
};

#endif