#include "Game.h"
int main(int argc, char* argv[])
{
    Game game;
    if (!game.init()) {
        std::cerr << "Game initialization failed!" << std::endl;
        return -1;
    }
    game.runGameLoop();

    game.cleanup();
    return 0;
}
