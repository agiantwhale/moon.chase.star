#include <CxxTL/tri_logger.hpp>
#include "../System/GraphicsManager.hpp"
#include "../App/Game.hpp"

SINGLETON_CONSTRUCTOR(GraphicsManager),
                      IEventListener("GraphicsManager"),
                      _renderLayerStack()
{
}

SINGLETON_DESTRUCTOR(GraphicsManager)
{
    Unload();
}

void GraphicsManager::SetUpGraphics(void)
{
	TRI_LOG_STR("Graphics initialized.");
}

bool GraphicsManager::HandleEvent( const EventData& newevent )
{
    if( newevent.GetEventType() == Event_Unload )
    {
        Unload();
        return false;
    }

    if( newevent.GetEventType() == Event_RestartLevel )
    {
        Unload();
        return false;
    }

    return false;
}

void GraphicsManager::Unload(void)
{
    for(RenderLayerStack::iterator iter = _renderLayerStack.begin(); iter != _renderLayerStack.end(); iter++)
    {
        delete (*iter);
    }

    _renderLayerStack.clear();
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

RenderLayer* GraphicsManager::GetRenderLayer( unsigned int layer )
{
    if( layer >= _renderLayerStack.size() )
    {
		TRI_LOG_STR("Attempted to access a non-existent layer!");
    }

    return _renderLayerStack.at( layer );
}