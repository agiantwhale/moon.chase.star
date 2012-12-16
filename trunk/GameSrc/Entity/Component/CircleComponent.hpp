#ifndef CIRCLECOMPONENT_HPP
#define CIRCLECOMPONENT_HPP

#include "../../Interface/IRenderable.hpp"
#include <SFML/Graphics.hpp>

class CircleComponent : public IRenderable
{
public:
    CircleComponent(Entity* entity,unsigned int layer);
    virtual ~CircleComponent();

    virtual void Render(void);

protected:
    sf::CircleShape* _circleShape;
};

#endif
