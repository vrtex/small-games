#include "game.h"
#include <SFML/Graphics.hpp>
#include <ctime>
#include <fstream>

int main()
{
	srand((unsigned int)time(nullptr));
    sf::RenderWindow w;
    Game g(&w);
    g.runMainLoop();
    return 0;
}
