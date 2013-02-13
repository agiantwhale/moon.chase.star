#ifndef Conversion_h__
#define Conversion_h__

#include <SFML/System/Vector2.hpp>
#include <Box2D/Common/b2Math.h>

sf::Vector2f ToVector(const b2Vec2& vector)
{
	sf::Vector2f returnVector(vector.x, vector.y);
	return returnVector;
}

b2Vec2 ToVector(const sf::Vector2f& vector)
{
	b2Vec2 returnVector(vector.x,vector.y);
	return returnVector;
}


#endif // Conversion_h__
