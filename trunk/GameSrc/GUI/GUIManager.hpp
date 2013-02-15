#ifndef GUIManager_h__
#define GUIManager_h__

#include "../Base/Singleton.hpp"
#include "../Event/EventListener.hpp"

namespace Gwen
{
	namespace Renderer
	{
		class Base;
	}

	namespace Controls
	{
		class Canvas;
	}

	namespace Skin
	{
		class Base;
	}

	namespace Input
	{
		class SFML;
	}
}

namespace sb
{
	class GUIManager : public Singleton<GUIManager>, public EventListener
	{
		DEFINE_SINGLETON(GUIManager)

	public:
		virtual bool handleEvent(const EventData& theevent);
		void setUpGUI(void);
		void render(void);

		Gwen::Controls::Canvas* getCanvas() const { return m_gwenCanvas; }

	private:
		Gwen::Renderer::Base*		m_gwenRenderer;
		Gwen::Controls::Canvas*		m_gwenCanvas;
		Gwen::Skin::Base*			m_gwenSkin;
		Gwen::Input::SFML*			m_gwenInput;
	};
}

#endif // GUIManager_h__
