#ifndef PARTICLEWRAPPER_HPP
#define PARTICLEWRAPPER_HPP

#include "../../Interface/IRenderable.hpp"
#include "../../Entity/Entity.hpp"
#include <Thor/Particles.hpp>
#include <SFML/Graphics.hpp>

class ParticleWrapper : public IRenderable
{
public:
	ParticleWrapper(Entity* const entity);
	virtual ~ParticleWrapper();

	virtual void Render(void);

protected:

};

#endif
