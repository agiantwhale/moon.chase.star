#include "../../Entity/Component/SpriteWrapper.hpp"

void sb::WorldSprite::draw( sf::RenderTarget& target, sf::RenderStates states ) const
{
	sf::Vector2f worldVector = getPosition();
	sf::Vector2f screenVector( SCREENWIDTH/2, SCREENHEIGHT/2 );

	screenVector.x += worldVector.x * RATIO;
	screenVector.y += worldVector.y * RATIO;

	setPosition(screenVector);

	sf::Sprite::draw(target,states);

	setPosition(worldVector);
}
