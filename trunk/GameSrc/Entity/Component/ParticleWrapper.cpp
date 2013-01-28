#include "../../App/Game.hpp"
#include "../../Entity/Component/ParticleWrapper.hpp"

ParticleWrapper::ParticleWrapper( const ITransform* transform )
	:	IRenderable(transform),
		_particleSystem(nullptr)
{

}

ParticleWrapper::~ParticleWrapper()
{
	delete _particleSystem;
}

void ParticleWrapper::Render( void )
{
	Game::GetInstance()->draw(*_particleSystem);
}

void ParticleWrapper::Update( float dt )
{
	if(_particleSystem)
	{
		sf::Time deltaTime = sf::seconds(dt);
		_particleSystem->update(deltaTime);
	}
}
