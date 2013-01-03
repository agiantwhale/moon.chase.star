#ifndef EVENTSDEF_H
#define EVENTSDEF_H

enum EventType
{
	Event_UNDEFINED = 0,

	//App related,
	Event_App,

	//GUI related,
	Event_GUI,

	//Physics related
	Event_BeginContact,
	Event_EndContact,
	Event_PreSolve,
	Event_PostSolve,
	Event_Simulate,

	Event_BallDied,
	Event_NewGame,
	Event_LevelCompleted,
	Event_RestartLevel,
	Event_NextLevel,
	Event_GameOver,
	Event_Unload,
	Event_ExitGame,
	NUM_EVENTS
};

#endif