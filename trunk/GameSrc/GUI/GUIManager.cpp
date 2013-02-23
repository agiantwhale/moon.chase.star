#include "GUIManager.hpp"
#include "../Base/Globals.hpp"
#include "../App/Game.hpp"

#include <Gwen/Gwen.h>
#include <Gwen/Renderers/SFML.h>
#include <Gwen/Controls/Canvas.h>
#include <Gwen/Skins/TexturedBase.h>
#include <Gwen/Input/SFML.h>

#include "../Event/AppEventData.hpp"

namespace sb
{
	SINGLETON_CONSTRUCTOR(GUIManager),
		EventListener(),
		m_gwenRenderer(nullptr),
		m_gwenCanvas(nullptr),
		m_gwenSkin(nullptr),
		m_gwenInput(nullptr)
	{
		addEventListenType(Event_App);
	}

	SINGLETON_DESTRUCTOR(GUIManager)
	{
		removeEventListenType(Event_App);

		delete m_gwenRenderer;
		delete m_gwenCanvas;
		delete m_gwenSkin;
		delete m_gwenInput;
	}

	void GUIManager::render(void)
	{
		Game::getInstance()->setView(Game::getInstance()->getDefaultView());

		m_gwenCanvas->RenderCanvas();
	}

	void GUIManager::setUpGUI( const TiXmlElement* element )
	{
		m_gwenRenderer = new Gwen::Renderer::SFML(*Game::getInstance());

		Gwen::Skin::TexturedBase* texturedSkin = new Gwen::Skin::TexturedBase(m_gwenRenderer);
		texturedSkin->Init( "Resource/Textures/GUISkin.png" );
		texturedSkin->SetDefaultFont( L"Resource/Fonts/Stroke-Bold.otf", 20 );
		m_gwenSkin = texturedSkin;

		m_gwenCanvas = new Gwen::Controls::Canvas( m_gwenSkin );
		m_gwenCanvas->SetSize( SCREENWIDTH, SCREENHEIGHT );

		m_gwenInput = new Gwen::Input::SFML;
		m_gwenInput->Initialize(m_gwenCanvas);

		TRI_LOG_STR("GUI initialized.");
	}

	bool GUIManager::handleEvent( const EventData& theevent )
	{
		if( theevent.getEventType() == Event_App)
		{
			const AppEventData& eventData = static_cast<const AppEventData&>(theevent);
			m_gwenInput->ProcessMessage(eventData.getAppEvent());
		}

		return false;
	}
}

