#ifndef NEXTLEVELEVENTDATA_HPP
#define NEXTLEVELEVENTDATA_HPP

#include <string>
#include "../Event/EventData.hpp"

class NextLevelEventData : public EventData
{
public:
	NextLevelEventData(const std::string& sceneFileName) : EventData(Event_NextLevel), _sceneFileName(sceneFileName)
	{
	}

	const std::string& GetSceneFileName(void) const {return _sceneFileName;}

private:
	std::string _sceneFileName;
};


#endif