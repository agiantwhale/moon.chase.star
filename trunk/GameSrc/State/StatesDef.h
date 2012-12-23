#ifndef STATESDEF_H
#define STATESDEF_H

enum StateType
{
	State_UNDEFINED = 0,
	State_Intro,
	State_MainMenu,
	State_Transition,
	State_Loading,
	State_InGame,
	State_Paused,
	State_Credits,
	NUM_STATES
};

#endif