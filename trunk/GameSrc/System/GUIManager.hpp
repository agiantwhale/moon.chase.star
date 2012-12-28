#ifndef GUIMANAGER_HPP
#define GUIMANAGER_HPP

#include "../Base/Singleton.hpp"

#include <glog/logging.h>
#include <SFML/Graphics.hpp>
#include <Gwen/Gwen.h>
#include <Gwen/Skin.h>
#include <Gwen/Input/SFML.h>
#include <Gwen/Controls/ListBox.h>

class GUIManager : public Singleton<GUIManager>, protected google::LogSink
{
    DEFINE_SINGLETON(GUIManager)

public:
    void SetUpGUI(void);
	void FeedEvent(sf::Event& sfEvent);
	void Render(void);

	Gwen::Controls::Canvas* GetCanvas() const { return _gwenCanvas; }

private:
    virtual void send(google::LogSeverity severity, const char* full_filename, const char* base_filename, int line, const struct ::tm* tm_time, const char* message, size_t message_len);

    Gwen::Renderer::Base*		_gwenRenderer;
    Gwen::Controls::Canvas*		_gwenCanvas;
	Gwen::Skin::Base*			_gwenSkin;
    Gwen::Input::SFML*			_gwenInput;
	Gwen::Controls::ListBox*	_gwenOuput;
};

#endif