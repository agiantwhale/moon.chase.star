#ifndef STARINITIALIZEEVENTDATA_HPP
#define STARINITIALIZEEVENTDATA_HPP

#include "../Event/EventData.hpp"

class StarEntity;

class StarInitializeEventData : public EventData
{
public:
	StarInitializeEventData(const StarEntity* entity) : EventData(Event_StarInitialize), _starEntity(entity)
	{

	}

	const StarEntity* GetStarEntity() const { return _starEntity; }

private:
	const StarEntity* _starEntity;
};

#endif