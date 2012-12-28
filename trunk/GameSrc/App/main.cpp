#include <assert.h>
#include <glog\logging.h>
#include "../App/Game.hpp"

int main(int argc,char *argv[])
{
    google::InitGoogleLogging(argv[0]);
    google::SetLogDestination(0, "Rebounce_Game" ); //INFO
    google::SetLogDestination(1, "Rebounce_Game" ); //WARNING
    google::SetLogDestination(2, "Rebounce_Game" ); //ERROR
    google::SetLogDestination(3, "Rebounce_Game" ); //FATAL
    google::SetLogFilenameExtension(".log");

    LOG(INFO) << "Game started.";

    Game::GetInstance()->Initialize();

    Game::GetInstance()->Start();

    google::ShutdownGoogleLogging();
    return 0;
}
