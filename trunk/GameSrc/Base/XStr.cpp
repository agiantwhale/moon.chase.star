#include "XStr.hpp"
#include <vadefs.h>
#include <stdarg.h>

typedef unsigned char       BYTE;
typedef unsigned short      WORD;
#define MAKEWORD(a, b)      ((WORD)(((BYTE)(((DWORD_PTR)(a)) & 0xff)) | ((WORD)((BYTE)(((DWORD_PTR)(b)) & 0xff))) << 8))
#define MAKELONG(a, b)      ((LONG)(((WORD)(((DWORD_PTR)(a)) & 0xffff)) | ((DWORD)((WORD)(((DWORD_PTR)(b)) & 0xffff))) << 16))
#define LOWORD(l)           ((WORD)(((DWORD_PTR)(l)) & 0xffff))
#define HIWORD(l)           ((WORD)((((DWORD_PTR)(l)) >> 16) & 0xffff))
#define LOBYTE(w)           ((BYTE)(((DWORD_PTR)(w)) & 0xff))
#define HIBYTE(w)           ((BYTE)((((DWORD_PTR)(w)) >> 8) & 0xff))

XStr::XStr( int hashlen, int len, int key, ... )
{
	va_list vl;

	va_start( vl, key );

	m_hashLength = hashlen;
	m_strLength  = len;
	m_key   = key;

	int nBytes = 0;
	int nDecryptedBytes = 0;

	m_charList = new DWORD_PTR[ len ];

	for( int i = 0; i < m_hashLength; i++ )
	{
		DWORD_PTR dwHash = va_arg( vl, DWORD_PTR );

		for( int j = 0; j < 4; j++ )
		{
			WORD wWord = HIWORD( dwHash );

			if( j >= 2 ) 
				wWord = LOWORD( dwHash );

			switch( j )
			{
			case 0:
				m_charList[ nDecryptedBytes ] = HIBYTE( wWord );
				break;
			case 1:
				m_charList[ nDecryptedBytes ] = LOBYTE( wWord );
				break;
			case 2:
				m_charList[ nDecryptedBytes ] = HIBYTE( wWord );
				break;
			case 3:
				m_charList[ nDecryptedBytes ] = LOBYTE( wWord );
				break;    
			}

			nDecryptedBytes++;

			if( nDecryptedBytes >= len )
				break;
		}   
	}  

	va_end( vl );
}

XStr::~XStr( )
{
	delete [] m_retBuf;

	m_retBuf = nullptr;
}

char * XStr::c( )
{
	m_retBuf = new char[ m_strLength + 1 ];

	for( int i = 0; i < m_strLength; i++ )
	{
		m_retBuf[ i ] = (char)m_charList[ i ];

		m_retBuf[ i ] ^= m_key;
		m_key += 1;
		m_key %= 256;
	}

	m_retBuf[ m_strLength ] = '\0';

	delete [] m_charList;

	m_charList = nullptr;

	return m_retBuf;
}