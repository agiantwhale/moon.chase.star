#ifndef BGMENTITY_HPP
#define BGMENTITY_HPP

#include <SFML/Audio.hpp>
#include "../Entity/Entity.hpp"

class BGMEntity : public Entity
{
	DEFINE_ENTITY( BGMEntity, Entity, 'BGME')

public:
	virtual void Initialize( const TiXmlElement *propertyElement = NULL );

private:
	sf::Music*	_backgroundMusic;
};

#endif
