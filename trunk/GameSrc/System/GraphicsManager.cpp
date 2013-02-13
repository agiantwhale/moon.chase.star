#include <CxxTL/tri_logger.hpp>
#include "GraphicsManager.hpp"
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

	void GraphicsManager::setUpGraphics(void)
	{
		TRI_LOG_STR("Graphics initialized.");
	}

	bool GraphicsManager::handleEvent( const EventData& newevent )
	{
		switch (newevent.GetEventType())
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

	void GraphicsManager::AddRenderable(IRenderable* renderable)
	{
		unsigned int layer = renderable->GetRenderLayer();
		while( (layer+1) > _renderLayerStack.size() )
		{
			_renderLayerStack.push_back(new RenderLayer);
		}

		_renderLayerStack.at(layer)->AddRenderable(renderable);
	}

	void GraphicsManager::RemoveRenderable(IRenderable* renderable)
	{
		unsigned int layer = renderable->GetRenderLayer();

		if( layer < _renderLayerStack.size() )
		{
			_renderLayerStack.at(layer)->RemoveRenderable( renderable );
		}
	}

	void GraphicsManager::Render(void)
	{
		for(RenderLayerStack::iterator iter = _renderLayerStack.begin(); iter != _renderLayerStack.end(); iter++)
		{
			RenderLayer* renderLayer = (*iter);
			renderLayer->Render();
		}
	}

	RenderLayer* GraphicsManager::GetRenderLayer( unsigned int layer ) const
	{
		if( layer >= _renderLayerStack.size() )
		{
			TRI_LOG_STR("Attempted to access a non-existent layer!");
		}

		return _renderLayerStack.at( layer );
	}
}