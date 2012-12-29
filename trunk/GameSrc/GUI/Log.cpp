#include "Log.hpp"

GWEN_CONTROL_CONSTRUCTOR(LogWindowControl)
{
	m_Title->SetText("Log");
	SetSize(600,200);
	SetPos(100,50);
	SetClosable(false);

	_gwenOuput = new Gwen::Controls::ListBox(this);
	_gwenOuput->Dock(Gwen::Pos::Fill);
}