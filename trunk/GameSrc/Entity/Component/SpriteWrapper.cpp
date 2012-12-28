#include "../../Entity/Component/SpriteWrapper.hpp"
#include "../../Entity/Entity.hpp"
#include "../../App/Game.hpp"

SpriteWrapper::SpriteWrapper(Entity* const entity) : IRenderable(entity), _sprite(nullptr)
{
}

SpriteWrapper::SpriteWrapper(const SpriteWrapper& wrapper, Entity* const entity) : IRenderable( wrapper, entity )
{
    if( wrapper._sprite )
        this->_sprite = new sf::Sprite(*wrapper._sprite->getTexture(), wrapper._sprite->getTextureRect());
}

SpriteWrapper::~SpriteWrapper()
{
    delete _sprite;
}

void SpriteWrapper::Render()
{
    if(_sprite)
    {
        Vec2D screenPosition = WorldToScreen(GetTransform()->GetPosition());
        _sprite->setPosition(screenPosition);
        _sprite->setRotation(GetTransform()->GetRotation());
        Game::GetInstance()->draw(*_sprite);
    }
    else
    {
        LOG(WARNING) << "Sprite component is uninitialized!";
    }
}
