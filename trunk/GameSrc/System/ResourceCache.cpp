#include "../App/Game.hpp"
#include "../System/ResourceCache.hpp"

SINGLETON_CONSTRUCTOR( ResourceCache )
{
	setReleaseStrategy(thor::Resources::ExplicitRelease);
	setLoadingFailureStrategy(thor::Resources::ReturnNullPointer);
}

SINGLETON_DESTRUCTOR( ResourceCache )
{
}