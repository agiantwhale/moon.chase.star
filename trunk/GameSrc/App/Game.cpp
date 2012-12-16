#include "../App/Game.hpp"
#include "../System/GraphicsManager.hpp"
#include "../System/EntityManager.hpp"
#include "../System/PhysicsManager.hpp"
#include "../System/ResourceManager.hpp"
#include "../System/SceneManager.hpp"

#include "../Tile/Tile.hpp"


SINGLETON_CONSTRUCTOR( Game ), sf::RenderWindow(sf::VideoMode(SCREENWIDTH, SCREENHEIGHT),"Bounce",sf::Style::Close), _isRunning( true ), _gameClock()
{
}

SINGLETON_DESTRUCTOR( Game )
{
}

void Game::Initialize( void )
{
	PhysicsManager::GetInstance()->SetUpPhysics();

	Tile::RegisterTileset("Rect", "Resource/Ogmo/Tiles/Rect.png");

	LOG(INFO) << "Game initialized.";
}

void Game::Start( void )
{
    _gameClock.restart();

	while(_isRunning)
	{
		if( !HandleEvents() ) return;
		Update();
		Render();
	}
}

bool Game::HandleEvents(void)
{
    sf::Event windowEvent;
    while(pollEvent(windowEvent))
    {
        if(windowEvent.type == sf::Event::Closed)
        {
            close();
            _isRunning = false;
            return false;
        }
    }

    return true;
}

void Game::Update(void)
{
	float deltaTime = _gameClock.restart().asSeconds();

	PhysicsManager::GetInstance()->FixedUpdate( deltaTime );
	EntityManager::GetInstance()->Update( deltaTime );
}

void Game::Render( void )
{
    clear();

	GraphicsManager::GetInstance()->Render();
	PhysicsManager::GetInstance()->Render();

	display();
}
