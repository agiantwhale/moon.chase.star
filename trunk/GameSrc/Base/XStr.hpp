#ifndef XStr_h__
#define XStr_h__

#include <basetsd.h>

class XStr
{
public:
	XStr( int hashlen, int strlen, int key, ... );
	virtual ~XStr( );
	char * c();

private:
	DWORD_PTR * m_charList;
	char * m_retBuf;
	int m_strLength;
	int m_hashLength;
	int m_key;
};

#endif // XStr_h__
