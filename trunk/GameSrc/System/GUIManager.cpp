#include "../Base/Globals.hpp"
#include "../App/Game.hpp"
#include "../System/GUIManager.hpp"
#include "../GUI/Log.hpp"

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
    texturedSkin->Init( "Resource/Textures/GUISkin.png" );
    texturedSkin->SetDefaultFont( L"Resource/Fonts/HeumToda.ttf", 20 );
	_gwenSkin = texturedSkin;

    _gwenCanvas = new Gwen::Controls::Canvas( _gwenSkin );
    _gwenCanvas->SetSize( SCREENWIDTH, SCREENHEIGHT );

    _gwenInput = new Gwen::Input::SFML;
    _gwenInput->Initialize(_gwenCanvas);

	TRI_LOG_STR("GUI initialized.");
}