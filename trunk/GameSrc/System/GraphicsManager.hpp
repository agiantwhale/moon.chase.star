#ifndef GRAPHICSMANAGER_HPP
#define GRAPHICSMANAGER_HPP

#include <list>
#include <vector>
#include <string>

#include "../Base/Singleton.hpp"
#include "../System/RenderLayer.hpp"

class GraphicsManager : public Singleton<GraphicsManager>
{
	DEFINE_SINGLETON( GraphicsManager );

public:
    void AddRenderable(IRenderable* renderable);
    void RemoveRenderable(IRenderable* renderable);

    void Render();
	void Unload();

private:
    typedef std::vector<RenderLayer*> RenderLayerStack;
    RenderLayerStack _renderLayerStack;
};

#endif
