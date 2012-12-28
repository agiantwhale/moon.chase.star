#ifndef GUIMANAGER_HPP
#define GUIMANAGER_HPP

#include "../Base/Globals.hpp"
#include "../App/Game.hpp"
#include "../Base/Singleton.hpp"

#include <glog/logging.h>
#include <Gwen/Gwen.h>
#include <Gwen/Renderers/SFML.h>
#include <Gwen/Controls/Canvas.h>
#include <Gwen/Skins/TexturedBase.h>
#include <Gwen/Input/SFML.h>

class GUIManager : public Singleton<GUIManager>, protected google::LogSink
{
    DEFINE_SINGLETON(GUIManager)

public:
    void SetUpGUI(void);

private:
    virtual void send(google::LogSeverity severity, const char* full_filename, const char* base_filename, int line, const struct ::tm* tm_time, const char* message, size_t message_len);

    Gwen::Renderer::SFML*		_gwenRenderer;
    Gwen::Controls::Canvas*		_gwenCanvas;
    Gwen::Skin::TexturedBase*	_gwenSkin;
    Gwen::Input::SFML*			_gwenInput;
};

#endif