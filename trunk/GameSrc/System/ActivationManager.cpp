#include "ActivationManager.hpp"
#include "../Base/XStr.hpp"

#include <cryptlib.h>
#include <Base32.h>			   // Base32
#include <hex.h>			// Hex
#include <aes.h>        // AES
#include <modes.h>      // CBC_Mode< >
#include <filters.h>    // StringSource and
#include <winerror.h>
#include <tchar.h>
#include <shlobj.h>
#include <shlwapi.h>
// StreamTransformation

namespace sb
{
	SINGLETON_CONSTRUCTOR(ActivationManager)
	{
		m_genuine = false;
	}

	SINGLETON_DESTRUCTOR(ActivationManager)
	{

	}
}

void sb::ActivationManager::setUpActivation( const TiXmlElement* element )
{
	if(element == nullptr) return;
	std::string activationKey = "";
	element->QueryStringAttribute("Key", &activationKey);

	activate(activationKey);
}

bool sb::ActivationManager::isGenuine()
{
	return m_genuine;
}

void sb::ActivationManager::activate( const std::string& activationKey )
{
	if( activationKey.size() != 34 )
	{
		return;
	}

	std::string encodedText = activationKey;
	std::string recoveredText = "";

	// Key and IV setup
	byte key[ CryptoPP::AES::DEFAULT_KEYLENGTH ] =
	{ 0x6D, 0xF5, 0x87, 0xF8, 0xB3, 0x42, 0x66, 0x84, 0x8A, 0xBE, 0xA1, 0x1C, 0x0C, 0x44, 0x62, 0x07 };

	byte  iv[ CryptoPP::AES::BLOCKSIZE ] =
	{ 0xDB, 0x3C, 0x2E, 0xAD, 0xFF, 0x05, 0x40, 0x0D, 0xA8, 0xB0, 0x7D, 0x2F, 0x9F, 0x9A, 0xA7, 0xC3 };

	// Decryptor
	CryptoPP::CBC_Mode<CryptoPP::AES>::Decryption
		Decryptor( key, sizeof(key), iv );

	encodedText = encodedText.substr( 0, encodedText.length() - 2 );

	CryptoPP::StringSource( encodedText, true,
		new CryptoPP::Base32Decoder(
		new CryptoPP::StreamTransformationFilter( Decryptor,
		new CryptoPP::StringSink( recoveredText )
		) // StreamTransformationFilter
		) // Base32Decoder
		); // StringSource

	if(recoveredText.length() >= 4)
	{
		recoveredText = recoveredText.substr( 4 );

		std::string secretString = XStr( /*ckij*/ 0x01, 0x04, 0x43, 0x202F2C2C ).c();
		//std::string secretString = "ckij";
		if( recoveredText.compare(secretString) == 0 )
		{
			m_genuine = true;
		}
	}
}

void sb::ActivationManager::writeToFile( const std::string& activationKey )
{
	TCHAR szPath[MAX_PATH];
	TiXmlDocument document;

	if(SUCCEEDED(

		SHGetFolderPath(NULL, 
		CSIDL_PERSONAL|CSIDL_FLAG_CREATE, 
		NULL, 
		0, 
		szPath))) 
	{
		PathAppend(szPath, TEXT("mcs.xml"));

		char ch[MAX_PATH];
		char DefChar = ' ';
		WideCharToMultiByte(CP_ACP,0,szPath,-1, ch,260,&DefChar, NULL);

		TiXmlDocument document(ch);
		if(document.LoadFile())
		{
			TiXmlElement* scenesElement = document.FirstChildElement("Activation");
			if(scenesElement)
			{
				scenesElement->SetAttribute("Key", activationKey);
			}

			document.SaveFile();
		}
	}
}

