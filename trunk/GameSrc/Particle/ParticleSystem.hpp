#ifndef PARTICLE_H
#define PARTICLE_H

#include <SFML/Graphics.hpp>
#include <list>
#include "../Interface/ITransform.hpp"
#include "../Interface/IRenderable.hpp"

namespace Shape { enum { CIRCLE, SQUARE }; }

struct Particle
{
	Vec2D pos; // Position
	Vec2D vel; // Velocity
	sf::Color color;  // RGBA
	float activeTime;
};

typedef std::list<Particle*>::iterator ParticleIterator;

class ParticleSystem : public ITransform, public IRenderable
{
public:
	enum ParticleShape
	{
		Particle_Circle,
		Particle_Square
	};

	ParticleSystem(const Vec2D& size);
	~ParticleSystem();

	void Fuel(int particles);
	void Update(float dt);
	void Render(void);
	void Remove(void);

	float GetGravityScale() const { return _gravityScale; }
	void SetGravityScale(float val) { _gravityScale = val; }

	bool GetDissolve() const { return _dissolve; }
	void SetDissolve(bool val) { _dissolve = val; }

	float GetDissolutionRate() const { return _dissolutionRate; }
	void SetDissolutionRate(float val) { _dissolutionRate = val; }

	float GetActiveTime() const { return _activeTime; }
	void SetActiveTime(float val) { _activeTime = val; }

	ParticleShape GetParticleShape() const { return _particleShape; }
	void SetParticleShape(ParticleShape val) { _particleShape = val; }

private:
	float		 _gravityScale;
	float		 _speedScale;
	sf::Texture* _particleTexture;
	sf::Sprite*  _particleSprite;
	bool _dissolve;
	float _dissolutionRate;
	float _activeTime;
	ParticleShape _particleShape;
	std::list<Particle*> _particles;
};

#endif