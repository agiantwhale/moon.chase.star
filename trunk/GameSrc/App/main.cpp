#include <CxxTL/tri_logger.hpp>
#include "../App/Game.hpp"
#include "../System/SceneManager.hpp"

#include <Thor/Graphics.hpp>

int main(int argc,char *argv[])
{
	sb::Game::getInstance()->initialize();
	sb::Game::getInstance()->start();

	sb::Game::getInstance()->GetAppDocument().SaveFile();

    return 0;
}
