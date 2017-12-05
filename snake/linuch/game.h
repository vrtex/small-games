#ifndef GAME_H
#define GAME_H

#include <vector>
#include <SFML/Graphics.hpp>
#include "gamestate.h"

class Game
{
public:
    Game(sf::RenderWindow *w);
    void runMainLoop();
	void updateStates();
	void changeRequest(GameState *newState);
	void addRequest(GameState *newState);
	void deleteRequest(int count);
    ~Game();
private:
	std::vector<GameState *> states;
	GameState *toPush, *toChange;
	int deleteState;
    sf::RenderWindow *window;
};

#endif /* end of include guard: GAME_H */
