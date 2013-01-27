#ifndef RESOURCECACHE_HPP
#define RESOURCECACHE_HPP

#include <Thor/Resources.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <string>
#include "../Base/Singleton.hpp"
#include "../Interface/IEventListener.hpp"

class ResourceCache : public Singleton<ResourceCache>, public thor::MultiResourceCache
{
	DEFINE_SINGLETON(ResourceCache)
};

#endif // RESOURCEMANAGER_H_INCLUDED
