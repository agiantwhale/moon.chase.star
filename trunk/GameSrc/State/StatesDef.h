#ifndef StatesDef_h__
#define StatesDef_h__

namespace sb
{
	enum StateType
	{
		State_UNDEFINED = 0,
		State_Intro,
		State_MainMenu,
		State_Transition,
		State_Loading,
		State_InGame,
		State_GameWin,
		State_Paused,
		State_Credits,
		NUM_STATES
	};
}

#endif // StatesDef_h__
