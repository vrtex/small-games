#include <SFML/Graphics.hpp>
#include "cellfield.h"
#include <cstdlib>
#include <ctime>

int main()
{
    srand(time(nullptr));
    sf::RenderWindow window(sf::VideoMode(400, 450), "LEL!");
    sf::Event e;
    sf::RectangleShape hud(sf::Vector2f(400, 50));
    CellField *field = new CellField(0, 50, &window);
    hud.setPosition(0, 0);
    hud.setFillColor(sf::Color::Black);
    while (window.isOpen())
    {
        while (window.pollEvent(e))
        {
            if (e.type == sf::Event::Closed)
                window.close();
            if(field->getInput(e)) continue;
        }
        window.clear(sf::Color::Red);
        window.draw(hud);
        if(field->update())
        {
            delete field;
            field = new CellField(0, 50, &window);
        }
        field->draw();
        window.display();
    }

    return 0;
}
