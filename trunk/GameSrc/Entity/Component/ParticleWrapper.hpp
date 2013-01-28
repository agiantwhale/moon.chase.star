#ifndef PARTICLEWRAPPER_HPP
#define PARTICLEWRAPPER_HPP

#include "../../Interface/IRenderable.hpp"
#include "../../Interface/ITransform.hpp"
#include <Thor/Particles.hpp>

class ParticleWrapper : public IRenderable
{
public:
	ParticleWrapper(const ITransform* transform);
	virtual ~ParticleWrapper();

	virtual void Render(void);
	void Update(float dt);

	thor::ParticleSystem* GetParticleSystem() const { return _particleSystem; }
	void SetParticleSystem(thor::ParticleSystem* val) { _particleSystem = val; }

private:
	thor::ParticleSystem* _particleSystem;
};

#endif