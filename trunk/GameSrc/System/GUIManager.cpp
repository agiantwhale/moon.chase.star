#include "../Base/Globals.hpp"
#include "../App/Game.hpp"
#include "../System/GUIManager.hpp"

#include <Gwen/Gwen.h>
#include <Gwen/Renderers/SFML.h>
#include <Gwen/Controls/Canvas.h>
#include <Gwen/Skins/TexturedBase.h>
#include <Gwen/Input/SFML.h>

#include <Gwen/Controls/WindowControl.h>
#include <Gwen/Controls/ListBox.h>

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

void GUIManager::FeedEvent(sf::Event& sfEvent)
{
	_gwenInput->ProcessMessage(sfEvent);
}

void GUIManager::Render(void)
{
	_gwenCanvas->RenderCanvas();
}

void GUIManager::SetUpGUI(void)
{
    _gwenRenderer = new Gwen::Renderer::SFML(*Game::GetInstance());

    Gwen::Skin::TexturedBase* texturedSkin = new Gwen::Skin::TexturedBase(_gwenRenderer);
    texturedSkin->Init( "DefaultSkin.png" );
    texturedSkin->SetDefaultFont( L"OpenSans.ttf", 11 );
	_gwenSkin = texturedSkin;

    _gwenCanvas = new Gwen::Controls::Canvas( _gwenSkin );
    _gwenCanvas->SetSize( SCREENWIDTH, SCREENHEIGHT );

    _gwenInput = new Gwen::Input::SFML;
    _gwenInput->Initialize(_gwenCanvas);

    //TODO: Create a debug console!
	Gwen::Controls::WindowControl* windowControl = new Gwen::Controls::WindowControl( _gwenCanvas );
	windowControl->SetSize(600,200);
	windowControl->SetPos(100,50);
	windowControl->SetClosable(false);

	_gwenOuput = new Gwen::Controls::ListBox(windowControl);
	_gwenOuput->Dock(Gwen::Pos::Fill);

	google::AddLogSink(this);
}

void GUIManager::send(google::LogSeverity severity, const char* full_filename, const char* base_filename, int line, const struct ::tm* tm_time, const char* message, size_t message_len)
{
    std::string logString = ToString(severity,base_filename,line,tm_time,message,message_len);
	_gwenOuput->AddItem(logString);
}