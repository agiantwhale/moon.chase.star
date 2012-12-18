#ifndef GRAPHICSMANAGER_HPP
#define GRAPHICSMANAGER_HPP

#include <list>
#include <vector>
#include <string>
#include <Light\LightManager.h>

#include "../Base/Singleton.hpp"
#include "../System/RenderLayer.hpp"
#include "../Interface/IEventListner.hpp"

class GraphicsManager : public Singleton<GraphicsManager>, public IEventListener
{
	DEFINE_SINGLETON( GraphicsManager );

public:
	virtual const std::string& GetEventListenerName( void ) { return "GraphicsManager"; }
	virtual bool HandleEvent( const EventData& newevent );

    void AddRenderable(IRenderable* const renderable);
    void RemoveRenderable(IRenderable* const renderable);

    void Render();
	void Unload();

	void SetUpGraphics(void);

private:
    typedef std::vector<RenderLayer*> RenderLayerStack;
    RenderLayerStack _renderLayerStack;
};

#endif
