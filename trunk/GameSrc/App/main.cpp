#include <CxxTL/tri_logger.hpp>
#include "../App/Game.hpp"

int main(int argc,char *argv[])
{
    Game::GetInstance()->Initialize();
    Game::GetInstance()->Start();

    return 0;
}
