#include "pch.h"
#include "Game.h"
#include "FPS.h"

int main()
{
    FPS fps;
    Game game;

    sf::Clock deltaClock;

    while (game.getWindow().isOpen()) {
        fps.update();
        GameSettings::DELTA_TIME = deltaClock.restart().asSeconds();
        game.update(GameSettings::DELTA_TIME);
        game.render();
        
        if(GameSettings::PRINT_FPS_TO_CONSOLE)
            std::cout << fps.getFPS() << std::endl;
    }
    
    return 0;
}