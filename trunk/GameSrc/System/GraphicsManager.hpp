#ifndef GRAPHICSMANAGER_HPP
#define GRAPHICSMANAGER_HPP

#include <list>
#include <vector>
#include <string>
#include <Light\LightManager.h>

#include "../Base/Singleton.hpp"
#include "../System/RenderLayer.hpp"
#include "../Interface/IEventListner.hpp"

class GraphicsManager : public Singleton<GraphicsManager>, private IEventListener
{
    DEFINE_SINGLETON( GraphicsManager );

public:
    void AddRenderable(IRenderable* const renderable);
    void RemoveRenderable(IRenderable* const renderable);

    RenderLayer* GetRenderLayer( unsigned int layer );

    void Render();
    void Unload();

    void SetUpGraphics(void);

private:
	virtual bool HandleEvent( const EventData& newevent );

    typedef std::vector<RenderLayer*> RenderLayerStack;
    RenderLayerStack _renderLayerStack;
};

#endif
