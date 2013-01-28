#ifndef SPRITEWRAPPER_HPP
#define SPRITEWRAPPER_HPP

#include "../../Interface/IRenderable.hpp"
#include "../../Entity/Entity.hpp"
#include <SFML/Graphics.hpp>

class SpriteWrapper : public IRenderable
{
public:
    SpriteWrapper(const ITransform* entity);
    virtual ~SpriteWrapper();

    virtual void Render(void);

	sf::Sprite* GetSprite() const { return _sprite; }
	void SetSprite(sf::Sprite* val) { _sprite = val; }

protected:
    sf::Sprite* _sprite;
};

#endif
