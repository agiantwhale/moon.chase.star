#include "../Base/Globals.hpp"
#include "../App/Game.hpp"
#include "../System/RenderLayer.hpp"

void Camera::Transform( void ) const
{
    sf::View cameraView;
    cameraView.setCenter( SCREENWIDTH/2 + GetPosition().x * RATIO, SCREENHEIGHT/2 - GetPosition().y * RATIO);
    cameraView.setRotation(GetRotation());
    cameraView.setSize(sf::Vector2f(GetScale().x * SCREENWIDTH,GetScale().y * SCREENHEIGHT));

    Game::GetInstance()->setView(cameraView);
}

Camera::Camera() : ITransform()
{
	SetScale(Vec2D(1.0f,1.0f));
}

RenderLayer::~RenderLayer()
{
    //This shouldn't usually be called since Renderable will be deleted anyways.
    //Just for tiles.
    /*
    for( std::list<IRenderable*>::iterator iter = begin(); iter != end(); iter++ )
    {
    	IRenderable* renderable = (*iter);
    	delete renderable;
    }
    */
}

void RenderLayer::Render( void )
{
    _renderCamera.Transform();

    for( iterator iter = begin(); iter != end(); iter++ )
    {
        IRenderable* renderable = (*iter);
		if(!renderable->IsHidden())
			renderable->Render();
    }
}

void RenderLayer::AddRenderable( IRenderable* const renderable )
{
    push_back( renderable );
}

void RenderLayer::RemoveRenderable( IRenderable* const renderable )
{
    if( !empty() )
        remove( renderable );
}