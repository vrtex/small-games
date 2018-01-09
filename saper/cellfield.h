#include "cell.h"
#include <vector>
#include <set>

class CellField
{
public:
    CellField(int x, int y, sf::RenderWindow *w);
    bool getInput(sf::Event &e);
    bool update();
    void draw();
    void flood(Cell *);
private:
    int x, y;
    sf::RenderWindow *window;
    bool gameOver;
    std::vector<Cell> cells;
    sf::Font font;
    sf::FloatRect field;
};

bool vContains(std::vector<Cell *> &v, Cell *toFind);
