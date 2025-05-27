#include <SFML/Graphics.hpp>
#include "Game.h"
#include "FPS.h"

int main()
{
    FPS fps;
    Game game;

    sf::Clock deltaClock;

    while (game.getWindow().isOpen()) {
        fps.update();
        game.update(deltaClock.restart().asSeconds());
        game.render();
        //std::cout << fps.getFPS() << std::endl;
    }
    
    return 0;
}