#include "SFML/Graphics.hpp"
#include <iostream>

class Cell
{
public:
    Cell(int x, int y, sf::RenderWindow *w, sf::Font &f);
    int getInput(sf::Event &e);
    void draw();
    int click();
    void rClick();
    bool isBomb();
    void setBombs(int i);
    void addNeighbor(Cell *);
    std::vector<Cell *> & getNeighbors();
private:
    int x, y;
    sf::RenderWindow *window;
    sf::RectangleShape block;
    sf::CircleShape marker;
    bool hidden;
    bool marked;
    bool bomb;
    int closeBombs;
    static const int size;
    sf::Text prompt;
    std::vector<Cell *> neighbors;
};
