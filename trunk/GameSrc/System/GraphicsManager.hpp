#ifndef GRAPHICSMANAGER_HPP
#define GRAPHICSMANAGER_HPP

#include <list>
#include <vector>
#include <string>
#include <SFML/Graphics/Drawable.hpp>

#include "../Base/Singleton.hpp"
#include "../System/RenderLayer.hpp"
#include "../Event/EventListener.hpp"

namespace sb
{
	class GraphicsManager : public Singleton<GraphicsManager>, private EventListener
	{
		DEFINE_SINGLETON( GraphicsManager );

	public:
		void addDrawable(const sf::Drawable& drawable, unsigned int layer);
		void removeDrawable(const sf::Drawable& drawable, unsigned int layer);

		RenderLayer* getRenderLayer( unsigned int layer ) const;
		unsigned int getRenderLayerStackSize(void) const {return _renderLayerStack.size();}

		void render();
		void unload();

		void setUpGraphics(void);

	private:
		virtual bool handleEvent( const EventData& newevent );

		typedef std::vector<RenderLayer*> RenderLayerStack;
		RenderLayerStack m_renderLayerStack;
	};
}

#endif
