#include "cell.h"


Cell::Cell(int x, int y, sf::RenderWindow *w, sf::Font &f) :
    x(x), y(y), window(w),
    block(sf::Vector2f(size, size)), marker(size / 4),
    hidden(true), marked(false)
{
    block.setPosition(x, y);
    block.setFillColor(sf::Color::White);
    block.setOutlineThickness(-3);
    block.setOutlineColor(sf::Color::Black);

    marker.setPosition(x, y);
    marker.setFillColor(sf::Color::Blue);
    marker.move(size / 4, size / 4);

    prompt.setFont(f);
    prompt.setString("3");
    // prompt.setFillColor(sf::Color::Black);

    if((double)rand() / RAND_MAX < 0.4) bomb = true;
    else bomb = false;
}

int Cell::getInput(sf::Event &e)
{
    if(e.type == sf::Event::MouseButtonPressed && block.getGlobalBounds().contains(e.mouseButton.x, e.mouseButton.y))
    {
        if(e.mouseButton.button == sf::Mouse::Left)
            return click();
        else if(e.mouseButton.button == sf::Mouse::Right)
        {
            rClick();
            return 0;
        }
    }
    return 0;
}

void Cell::draw()
{
    window->draw(block);
    if(marked) window->draw(marker);
    if(!hidden && !bomb) window->draw(prompt);
}

int Cell::click()
{
    marked = false;
    if(!hidden) return 0;
    hidden = false;
    if(bomb)
    {
        block.setFillColor(sf::Color::Red);
        return 2;
    }
    prompt.setString(std::to_string(closeBombs));
    prompt.setOrigin(prompt.getGlobalBounds().width / 2, prompt.getGlobalBounds().height);
    prompt.setPosition(block.getPosition().x + size / 2, block.getPosition().y + size / 2);
    block.setFillColor(sf::Color::Green);
    return 1;
}

void Cell::rClick()
{
    if(!hidden) return;
    marked = !marked;
}

bool Cell::isBomb()
{
    return bomb;
}

void Cell::setBombs(int i)
{
    closeBombs = i;
}

void Cell::addNeighbor(Cell *n)
{
    neighbors.push_back(n);
}

std::vector<Cell *>  & Cell::getNeighbors()
{
    return neighbors;
}

const int Cell::size = 50;
