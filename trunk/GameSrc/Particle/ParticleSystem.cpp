#include <boost/random.hpp>
#include "../Particle/ParticleSystem.hpp"

float RandomFloat(float min, float max)
{
	boost::mt19937 rng;
	boost::uniform_real<float> u(min, max);
	boost::variate_generator<boost::mt19937&, boost::uniform_real<float> > gen(rng, u);
	return gen();
}

int RandomInt(int min, int max)
{
	boost::mt19937 rng;
	boost::uniform_int<int> u(min, max);
	boost::variate_generator<boost::mt19937&, boost::uniform_int<> > gen(rng, u);
	return gen();
}

ParticleSystem::ParticleSystem(const Vec2D& size) : ITransform(),
								   IRenderable(this),
								   _gravityScale(1.0f),
								   _speedScale(1.0f),
								   _particleTexture(nullptr),
								   _particleSprite(nullptr),
								   _dissolve(true),
								   _dissolutionRate(true),
								   _particleShape(Particle_Circle),
								   _activeTime(2.0f)
{
	Vec2D screenSize = WorldToScreen(size);
	int width = static_cast<int>(screenSize.x + 0.5);
	int height = static_cast<int>(screenSize.y + 0.5);

	_particleTexture = new sf::Texture();
	_particleTexture->create(width,height);

	_particleSprite = new sf::Sprite();
	_particleSprite->setTexture(*_particleTexture);
}

ParticleSystem::~ParticleSystem()
{
	for( ParticleIterator it = _particles.begin(); it != _particles.end(); it++ )
	{
		delete *it;
	}

	if(_particleTexture)
		delete _particleTexture;

	if(_particleSprite)
		delete _particleSprite;
}

void ParticleSystem::Fuel( int particles )
{
	float angle;
	Particle* particle = nullptr;
	for( int i = 0; i < particles; i++ )
	{
		particle = new Particle();
		particle->pos = GetPosition();

		switch( _particleShape )
		{
		case Shape::CIRCLE:
			angle = RandomFloat(0,6.2832f);
			particle->vel.x = RandomFloat( 0.0f, cos( angle ) );
			particle->vel.y = RandomFloat( 0.0f, sin( angle ) );
			break;
		case Shape::SQUARE:
			particle->vel.x = RandomFloat( -1.0f, 1.0f );
			particle->vel.y = RandomFloat( -1.0f, 1.0f );
			break;
		default:
			particle->vel.x = 0.5f; // Easily detected
			particle->vel.y = 0.5f; // Easily detected
		}

		if( particle->vel.x == 0.0f && particle->vel.y == 0.0f )
		{
			delete particle;
			continue;
		}

		particle->color.r = RandomInt( 0, 255 );
		particle->color.g = RandomInt( 0, 255 );
		particle->color.b = RandomInt( 0, 255 );
		particle->color.a = 255;

		particle->activeTime = _activeTime;
		_particles.push_back( particle );
	}
}

void ParticleSystem::Update( float dt )
{
	for( ParticleIterator it = _particles.begin(); it != _particles.end(); it++ )
	{
		(*it)->activeTime -= dt;

		(*it)->vel.y -= GRAVITY_ACCELERATION * _gravityScale * dt;
		(*it)->pos += (*it)->vel * dt * _speedScale;

		if( _dissolve )
		{
			(*it)->color.a = static_cast<int>(255 * (*it)->activeTime/_activeTime + 0.5f);
		}

		if( (*it)->activeTime <= 0 )
		{
			delete (*it);
			it = _particles.erase( it );
			if( it == _particles.end() ) return;
		}
	}
}

void ParticleSystem::Render( void )
{

}

void ParticleSystem::Remove( void )
{

}

