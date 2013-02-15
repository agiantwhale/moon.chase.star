#ifndef Updateable_h__
#define Updateable_h__

#include <SFML/System/Time.hpp>

namespace sb
{
	class Updateable
	{
		virtual ~Updateable() {}
		virtual void update( sf::Time deltaTime ) = 0;
	};
}


#endif // Updateable_h__
