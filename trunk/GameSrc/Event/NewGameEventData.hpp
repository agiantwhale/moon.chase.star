#ifndef NEWGAMEEVENTDATA_HPP
#define NEWGAMEEVENTDATA_HPP

#include <string>
#include "../Event/EventData.hpp"

class NewGameEventData : public EventData
{
public:
	NewGameEventData(const std::string& sceneFileName) : EventData(Event_NewGame), _sceneFileName(sceneFileName)
	{
	}

	const std::string& GetSceneFileName(void) const {return _sceneFileName;}

private:
	std::string _sceneFileName;
};


#endif