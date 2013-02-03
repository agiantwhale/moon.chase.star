#ifndef NEWGAMEEVENTDATA_HPP
#define NEWGAMEEVENTDATA_HPP

#include <string>
#include "../Event/EventData.hpp"

class NewGameEventData : public EventData
{
public:
	NewGameEventData(unsigned int sceneNum) : EventData(Event_NewGame), _sceneNum(sceneNum)
	{
	}

	unsigned int GetSceneNumber() const { return _sceneNum; }

private:
	unsigned int _sceneNum;
};


#endif