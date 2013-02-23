#ifndef GRAPHICSMANAGER_HPP
#define GRAPHICSMANAGER_HPP

#include <TinyXML/tinyxml.h>
#include <list>
#include <vector>
#include <string>
#include <SFML/Graphics/Drawable.hpp>

#include "../Base/Singleton.hpp"
#include "../Event/EventListener.hpp"

namespace sb
{
	class RenderLayer;

	class GraphicsManager : public Singleton<GraphicsManager>, private EventListener
	{
		DEFINE_SINGLETON( GraphicsManager );

	public:
		void addDrawable(const sf::Drawable& drawable, unsigned int layer);
		void removeDrawable(const sf::Drawable& drawable, unsigned int layer);

		RenderLayer* getRenderLayer( unsigned int layer ) const;
		unsigned int getRenderLayerStackSize(void) const {return m_renderLayerStack.size();}

		void render();
		void unload();

		void setUpGraphics( const TiXmlElement* element );

	private:
		virtual bool handleEvent( const EventData& newevent );

		typedef std::vector<RenderLayer*> RenderLayerStack;
		RenderLayerStack m_renderLayerStack;
	};
}

#endif
