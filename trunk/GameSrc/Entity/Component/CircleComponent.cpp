#include "../../Entity/Component/CircleComponent.hpp"
#include "../../Entity/Entity.hpp"
#include "../../Manager/CameraManager.hpp"
#include "../../App/Game.hpp"

CircleComponent::CircleComponent(Entity* entity,unsigned int layer) : IRenderable(entity,layer), _circleShape(NULL)
{
}

CircleComponent::~CircleComponent()
{
    delete _circleShape;
}

void CircleComponent::Render()
{
    if(_circleShape)
    {
        Game* game = Game::GetInstance();

        sf::Vector2f screenPosition;
        screenPosition = CameraManager::GetInstance()->WorldToScreen(GetEntity().GetPosition(),&screenPosition);
        _circleShape->setPosition(screenPosition);
        _circleShape->setRotation(GetEntity().GetAngle());
        game->draw(*_circleShape);
    }
    else
    {
        WARNING("Circle component is uninitialized!");
    }
}
