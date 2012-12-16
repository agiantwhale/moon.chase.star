#include "../Interface/IRenderable.hpp"
#include "../System/GraphicsManager.hpp"

IRenderable::IRenderable(ITransform* const transform) : _transform(transform), _isHidden(false), _renderLayer(0), _isRegistered( false )
{
}

IRenderable::IRenderable(const IRenderable& renderable, ITransform* const transform) : _transform(transform)
{
	this->_isHidden = renderable._isHidden;
	this->_renderLayer = 0;
}

IRenderable::~IRenderable()
{
	if( _isRegistered )
		UnregisterRenderable();
}

void IRenderable::RegisterRenderable( unsigned int renderLayer )
{
	_isRegistered = true;
	_renderLayer = renderLayer;
	GraphicsManager::GetInstance()->AddRenderable(this);
}

void IRenderable::UnregisterRenderable( void )
{
	_isRegistered = false;
	GraphicsManager::GetInstance()->RemoveRenderable(this);
}