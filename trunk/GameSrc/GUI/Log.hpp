#ifndef LOG_HPP
#define LOG_HPP

#include <Gwen/Controls/WindowControl.h>
#include <Gwen/Controls/ListBox.h>
//#include <logog/logog.hpp>

class LogWindowControl : private Gwen::Controls::WindowControl //, public logog::Target
{
	GWEN_CONTROL(LogWindowControl,Gwen::Controls::WindowControl);

private:
	//virtual int Output( const LOGOG_STRING &data );

	Gwen::Controls::ListBox*	_gwenOuput;
};

#endif // Log_h__
