#include <SFML/Graphics.hpp>
#include "Game.h"
#include "FPS.h"

int main()
{
    Game game;

    while (game.getWindow().isOpen()) {
        game.update();
        game.render();

    }
    
    return 0;
}