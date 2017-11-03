#include "game.h"

Game::Game(sf::RenderWindow *w) :
    window(w)
{
    w->create(sf::VideoMode(1024, 512), "Boob da snoot");
    w->setFramerateLimit(60);
	w->setKeyRepeatEnabled(false);

	toPush = nullptr;
	toChange = nullptr;
	deleteState = 0;

	addRequest(new Splash(this, window));
	updateStates();
}

void Game::runMainLoop()
{
    sf::Event e;
    while(window->isOpen())
    {
        window->clear(sf::Color(109, 106, 24));
        while(window->pollEvent(e))
        {
            if(e.type == sf::Event::Closed)
                window->close();
			else if(states.back()->getInput(e)) break;
        }
		updateStates();
		states.back()->update();
		for(auto &s : states)
		{
			s->draw();
		}
        window->display();
    }
}

void Game::updateStates()
{
	if(deleteState)
	{
		for(int i = 0; i < deleteState; ++i)
		{
			delete states.back();
			states.pop_back();
		}
		deleteState = 0;
		if(states.size())
			states.back()->activate();
	}
	if(toPush)
	{
		if(states.size())
			states.back()->deactivate();
		states.push_back(toPush);
		toPush = nullptr;
	}
	if(toChange)
	{
		if(states.size() > 0)
		{
			delete states.back();
			states.pop_back();
		}
		states.push_back(toChange);
		toChange = nullptr;
	}
}

void Game::changeRequest(GameState * newState)
{
	toChange = newState;
}

void Game::addRequest(GameState * newState)
{
	toPush = newState;
}

void Game::deleteRequest(int count)
{
	deleteState = count;
}

Game::~Game()
{
}
