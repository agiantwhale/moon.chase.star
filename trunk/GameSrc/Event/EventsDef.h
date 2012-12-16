#ifndef EVENTSDEF_H
#define EVENTSDEF_H

enum EventType
{
	UNDEFINED = 0,
	Event_BallDied,
	Event_LevelCompleted,
	Event_RestartLevel,
	Event_NextLevel,
	Event_GameOver,
	Event_ExitGame,
	NUM_MSG
};

#endif