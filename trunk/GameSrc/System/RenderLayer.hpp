#ifndef RENDERLAYER_HPP
#define RENDERLAYER_HPP

#include <list>

#include "../Interface/ITransform.hpp"
#include "../Interface/IRenderable.hpp"

class Camera : public ITransform
{
public:
	void Transform( void ) const;
};

class RenderLayer : private std::list<IRenderable*>
{
public:
	RenderLayer() : _renderCamera() {}
	~RenderLayer();

	void Render( void );
	void AddRenderable( IRenderable* renderable );
	void RemoveRenderable( IRenderable* renderable );

	Camera& GetCamera() { return _renderCamera; }
	void SetCamera( const Camera& camera ) { _renderCamera = camera; }

private:
	Camera _renderCamera;
};

#endif