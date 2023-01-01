#define NOMINMAX
#include <windows.h>
#include <string>
#include "Game.h"

int main()
{
    //Initizlize Random
    std::srand(static_cast<unsigned>(time(NULL)));

    std::cout <<
        "Welcome to 'Alien Exterminator', a game developed by Mark Onopchenko." << std::endl
        << "The goal of this game is to survive for as long as possible and kill as many aliens for the highest score!" << std::endl
        << "There are three types of aliens:" << std::endl
        << "Purple - deal ONE damage when they reach the bottom of the screen, no special properties" << std::endl
        << "Red - deal FIVE damage" << std::endl
        << "Green - give FIVE health if destroyed" << std::endl
        << "Best of luck!" << std::endl;

    // Initialize Game.cpp
    Game game;

    //Game loop
    while (game.running())
    {
        //Update
        game.update();

        //Render
        game.render();
    }
    system("pause");
    return 0;

}
