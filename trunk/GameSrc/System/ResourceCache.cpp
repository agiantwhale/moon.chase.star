#include "ResourceCache.hpp"

namespace sb
{
	SINGLETON_CONSTRUCTOR( ResourceCache )
	{
		setReleaseStrategy(thor::Resources::ExplicitRelease);
		setLoadingFailureStrategy(thor::Resources::ReturnNullPointer);
	}

	SINGLETON_DESTRUCTOR( ResourceCache )
	{
	}
}