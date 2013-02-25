#ifndef CreditsPage_h__
#define CreditsPage_h__

#include <Gwen/Controls/Base.h>

class CreditsPageControl : public Gwen::Controls::Base
{
	GWEN_CONTROL(CreditsPageControl,Gwen::Controls::Base);

private:
	Gwen::Font* m_creditsFont;
};

#endif // CreditsPage_h__
