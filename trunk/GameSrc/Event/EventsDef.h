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

	//Game related
	Event_PauseGame,
	Event_ResumeGame,
	Event_BallDied,
	Event_NewGame,
	Event_LevelCompleted,
	Event_RestartLevel,
	Event_NextLevel,
	Event_GameOver,
	Event_Unload,
	Event_ExitGame,

	//Gameplay related
	Event_StarArrived,
	Event_GravityChange,

	NUM_EVENTS
};

#endif