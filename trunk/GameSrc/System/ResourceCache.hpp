#ifndef ResourceCache_h__
#define ResourceCache_h__

#include <Thor/Resources.hpp>
#include <string>
#include "../Base/Singleton.hpp"

//pre-includes some important structures
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Audio/SoundBuffer.hpp>

namespace sb
{
	class ResourceCache : public Singleton<ResourceCache>, public thor::MultiResourceCache
	{
		DEFINE_SINGLETON(ResourceCache)
	};
}

#endif // ResourceCache_h__
