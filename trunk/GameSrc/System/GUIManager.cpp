#include "../System/GUIManager.hpp"

SINGLETON_CONSTRUCTOR(GUIManager),
                      _gwenRenderer(nullptr),
                      _gwenCanvas(nullptr),
                      _gwenSkin(nullptr),
                      _gwenInput(nullptr)
{
}

SINGLETON_DESTRUCTOR(GUIManager)
{
    delete _gwenRenderer;
    delete _gwenCanvas;
    delete _gwenSkin;
    delete _gwenInput;
}

void GUIManager::SetUpGUI(void)
{
    _gwenRenderer = new Gwen::Renderer::SFML(*Game::GetInstance());

    _gwenSkin = new Gwen::Skin::TexturedBase(_gwenRenderer);
    _gwenSkin->Init( "DefaultSkin.png" );
    _gwenSkin->SetDefaultFont( L"OpenSans.ttf", 11 );

    _gwenCanvas = new Gwen::Controls::Canvas( _gwenSkin );
    _gwenCanvas->SetSize( SCREENWIDTH, SCREENHEIGHT );

    _gwenInput = new Gwen::Input::SFML;
    _gwenInput->Initialize(_gwenCanvas);

    //TODO: Create a debug console!
}

void GUIManager::send(google::LogSeverity severity, const char* full_filename, const char* base_filename, int line, const struct ::tm* tm_time, const char* message, size_t message_len)
{
    std::string logString = ToString(severity,base_filename,line,tm_time,message,message_len);
}