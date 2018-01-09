#include "cellfield.h"


CellField::CellField(int x, int y, sf::RenderWindow *w) : x(x), y(y), window(w), gameOver(false)
{
    int cellSize = 50;
    int cols = window->getSize().x / cellSize;
    int rows = (window->getSize().y - 50) / cellSize;
    font.loadFromFile("font.ttf");
    for(int y = 0; y < rows; ++y)
    {
        for(int x = 0; x < cols; ++x)
        {
            cells.push_back(Cell(x * cellSize + this->x, y * cellSize + this->y, window, font));
        }
    }

    int closeBombs;
    for(int y = 0; y < rows; ++y)
    {
        for(int x = 0; x < cols; ++x)
        {
            closeBombs = 0;
            for(int yy = -1; yy <= 1; ++yy)
                for(int xx = -1; xx <= 1; ++xx)
                {
                    if(y + yy < 0 || y + yy >= rows || x + xx < 0 || x + xx >= cols || (xx == 0 && yy == 0)) continue;
                    if(cells[x + xx + (y + yy) * cols].isBomb()) ++closeBombs;
                    cells[x + y * cols].addNeighbor(&cells[x + xx + (y + yy) * cols]);
                }
            cells[x + y * cols].setBombs(closeBombs);
        }
    }

    field.left = x;
    field.top = y;
    field.width = cellSize * cols;
    field.height = cellSize * rows;
}

bool CellField::getInput(sf::Event &e)
{
    if(e.type == sf::Event::MouseButtonPressed && field.contains(e.mouseButton.x, e.mouseButton.y))
    {
        for(auto & c : cells)
        {
            int result = c.getInput(e);
            if(result == 1)
            {
                //empty
                flood(&c);
                return true;
            }
            else if(result == 2)
            {
                //boom game over
                gameOver = true;
                return true;
            }
        }


    }
    return false;
}

bool CellField::update()
{
    return gameOver;
}

void CellField::draw()
{
    for(auto & c : cells)
        c.draw();
}

void CellField::flood(Cell *first)
{
    Cell *current = first;
    std::vector<Cell *> visited;
    std::vector<Cell *> toVisit;
    for(auto & n : current->getNeighbors())
    {
        if(!n->isBomb()) toVisit.push_back(n);
    }
    visited.push_back(current);
    while(!toVisit.empty())
    {
        current = toVisit.back();
        toVisit.pop_back();
        current->click();
        for(auto & n : current->getNeighbors())
        {
            if(!n->isBomb() && !vContains(visited, n)) toVisit.push_back(n);
        }
        visited.push_back(current);
    }
}





bool vContains(std::vector<Cell *> &v, Cell *toFind)
{
    for(auto & a : v)
    {
        if(a == toFind) return true;
    }
    return false;
}
