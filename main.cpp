#include <SFML/Graphics.hpp>
#include "Game.h"
#include "FPS.h"

int main()
{
    FPS fps;
    Game game;

    while (game.getWindow().isOpen()) {
        fps.update();
        game.update();
        game.render();
        //std::cout << fps.getFPS() << std::endl;
    }
    
    return 0;
}