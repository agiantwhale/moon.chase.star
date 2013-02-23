#include <CxxTL/tri_logger.hpp>
#include "GraphicsManager.hpp"
#include "RenderLayer.hpp"
#include "../App/Game.hpp"

namespace sb
{
	SINGLETON_CONSTRUCTOR(GraphicsManager),
						  EventListener(),
						  m_renderLayerStack()
	{
		addEventListenType(Event_Unload);
		addEventListenType(Event_RestartLevel);

	}

	SINGLETON_DESTRUCTOR(GraphicsManager)
	{
		removeEventListenType(Event_Unload);
		removeEventListenType(Event_RestartLevel);
		unload();
	}

	void GraphicsManager::setUpGraphics( const TiXmlElement* element )
	{
		TRI_LOG_STR("Graphics initialized.");
	}

	bool GraphicsManager::handleEvent( const EventData& newevent )
	{
		switch (newevent.getEventType())
		{
		case Event_Unload:
			{
				unload();
				break;
			}

		case Event_RestartLevel:
			{
				for(RenderLayerStack::iterator iter = m_renderLayerStack.begin(); iter != m_renderLayerStack.end(); iter++)
				{
					RenderLayer* renderLayer = *iter;
					renderLayer->getCamera().setPosition(0.f,0.f);
					renderLayer->getCamera().setRotation(0.0f);
					renderLayer->getCamera().setScale(1.0f,1.0f);
				}

				break;
			}
		}

		return false;
	}

	void GraphicsManager::unload(void)
	{
		for(RenderLayerStack::iterator iter = m_renderLayerStack.begin(); iter != m_renderLayerStack.end(); iter++)
		{
			delete (*iter);
		}

		m_renderLayerStack.clear();
	}

	void GraphicsManager::render(void)
	{
		for(RenderLayerStack::iterator iter = m_renderLayerStack.begin(); iter != m_renderLayerStack.end(); iter++)
		{
			RenderLayer* renderLayer = (*iter);
			renderLayer->render();
		}
	}

	RenderLayer* GraphicsManager::getRenderLayer( unsigned int layer ) const
	{
		if( layer >= m_renderLayerStack.size() )
		{
			TRI_LOG_STR("Attempted to access a non-existent layer!");
		}

		return m_renderLayerStack.at( layer );
	}

	void GraphicsManager::addDrawable( const sf::Drawable& drawable, unsigned int layer )
	{
		while( (layer+1) > m_renderLayerStack.size() )
		{
			m_renderLayerStack.push_back(new RenderLayer);
		}

		m_renderLayerStack.at(layer)->addDrawable(drawable);
	}

	void GraphicsManager::removeDrawable( const sf::Drawable& drawable, unsigned int layer )
	{
		if( layer < m_renderLayerStack.size() )
		{
			m_renderLayerStack.at(layer)->removeDrawable(drawable);
		}
	}

}