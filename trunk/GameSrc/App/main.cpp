#include <assert.h>
#include <glog\logging.h>
#include "../App/Game.hpp"
#include "../System/SceneManager.hpp"

int main(int argc,char *argv[])
{
	google::InitGoogleLogging(argv[0]);
	google::SetLogDestination(google::INFO, "Rebounce_Game" );
	google::SetLogDestination(google::WARNING, "Rebounce_Game" );
	//google::SetLogDestination(google::ERROR, "Rebounce_Game" );
	google::SetLogDestination(google::FATAL, "Rebounce_Game" );
	google::SetLogFilenameExtension(".log");

	LOG(INFO) << "Game started.";

    Game::GetInstance()->Initialize();

    if( argc >= 2 )
    {
        std::string scenefile = argv[1];
		SceneManager::GetInstance()->LoadScene( scenefile );
    }

    Game::GetInstance()->Start();

	google::ShutdownGoogleLogging();
    return 0;
}
