#ifndef EVENTSDEF_H
#define EVENTSDEF_H

enum EventType
{
	Event_UNDEFINED = 0,
	Event_BallDied,
	Event_CameraMove,
	Event_LevelCompleted,
	Event_RestartLevel,
	Event_NextLevel,
	Event_GameOver,
	Event_Unload,
	Event_ExitGame,
	NUM_EVENTS
};

#endif