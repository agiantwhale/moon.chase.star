#include "../Interface/IRenderable.hpp"
#include "../System/GraphicsManager.hpp"

IRenderable::IRenderable(ITransform* const transform) : _transform(transform), _hidden(false), _renderLayer(0), _registered( false )
{
}

IRenderable::IRenderable(const IRenderable& renderable, ITransform* const transform) : _transform(transform)
{
    this->_hidden = renderable._hidden;
    this->_renderLayer = 0;
}

IRenderable::~IRenderable()
{
    if( _registered )
        UnregisterRenderable();
}

void IRenderable::RegisterRenderable( unsigned int renderLayer )
{
    _registered = true;
    _renderLayer = renderLayer;
    GraphicsManager::GetInstance()->AddRenderable(this);
}

void IRenderable::UnregisterRenderable( void )
{
    _registered = false;
    GraphicsManager::GetInstance()->RemoveRenderable(this);
}