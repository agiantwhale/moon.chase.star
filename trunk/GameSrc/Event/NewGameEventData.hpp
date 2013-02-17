#ifndef NEWGAMEEVENTDATA_HPP
#define NEWGAMEEVENTDATA_HPP

#include <string>
#include "EventData.hpp"


namespace sb
{
	class NewGameEventData : public EventData
	{
	public:
		NewGameEventData(unsigned int sceneNum) : EventData(Event_NewGame), m_sceneNum(sceneNum)
		{
		}

		unsigned int getSceneNumber() const { return m_sceneNum; }

	private:
		unsigned int m_sceneNum;
	};
}



#endif