#ifndef ActivationManager_h__
#define ActivationManager_h__

#include "../Base/Singleton.hpp"
#include <TinyXML/tinyxml.h>
#include <string>

namespace sb
{
	class ActivationManager : public Singleton<ActivationManager>
	{
		DEFINE_SINGLETON(ActivationManager)

	public:
		void setUpActivation(const TiXmlElement* element);
		void activate( const std::string& activationKey );
		bool isGenuine();

		void writeToFile( const std::string& activationKey );

	private:

		bool m_genuine;
	};
}

#endif // ActivationManager_h__
