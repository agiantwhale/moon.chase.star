#include "KinectSetUp.hpp"
#include <CxxTL/tri_logger.hpp>
#include <NiTE.h>

void SetUpOpenNI( void )
{
	if(nite::NiTE::initialize()!=nite::STATUS_OK)
	{
		TRI_LOG_STR("Couldn't set up Nite.");
	}
}
