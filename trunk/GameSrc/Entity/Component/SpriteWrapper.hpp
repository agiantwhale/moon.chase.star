#ifndef SPRITEWRAPPER_HPP
#define SPRITEWRAPPER_HPP

#include "../../Interface/IRenderable.hpp"
#include "../../Entity/Entity.hpp"
#include <SFML/Graphics.hpp>

class SpriteWrapper : public IRenderable
{
public:
    SpriteWrapper(Entity* const entity);
    SpriteWrapper(const SpriteWrapper& wrapper, Entity* const entity);
    virtual ~SpriteWrapper();

    virtual void Render(void);

    inline void SetSprite( sf::Sprite* sprite )
    {
        _sprite = sprite;
    }

protected:
    sf::Sprite* _sprite;
};

#endif
