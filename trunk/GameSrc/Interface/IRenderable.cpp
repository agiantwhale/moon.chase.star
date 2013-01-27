#include "../Interface/IRenderable.hpp"
#include "../System/GraphicsManager.hpp"

IRenderable::IRenderable(const ITransform* transform) : _transform(transform), _hidden(false), _renderLayer(0), _registered( false )
{
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