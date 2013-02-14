#ifndef ResourceCache_h__
#define ResourceCache_h__

#include <Thor/Resources.hpp>
#include <string>
#include "../Base/Singleton.hpp"

namespace sb
{
	class ResourceCache : public Singleton<ResourceCache>, public thor::MultiResourceCache
	{
		DEFINE_SINGLETON(ResourceCache)
	};
}

#endif // ResourceCache_h__
