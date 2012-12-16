#include <glog\logging.h>
#include "../System/GraphicsManager.hpp"

SINGLETON_CONSTRUCTOR(GraphicsManager), _renderLayerStack()
{
}

SINGLETON_DESTRUCTOR(GraphicsManager)
{
	Unload();
}

void GraphicsManager::Unload(void)
{
	for(RenderLayerStack::iterator iter = _renderLayerStack.begin(); iter != _renderLayerStack.end(); iter++)
    {
        delete (*iter);
    }
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
	_renderLayerStack.at(layer)->RemoveRenderable(renderable);
}

void GraphicsManager::Render(void)
{
    for(RenderLayerStack::iterator iter = _renderLayerStack.begin(); iter != _renderLayerStack.end(); iter++)
    {
		RenderLayer* renderLayer = (*iter);
        renderLayer->Render();
    }
}
