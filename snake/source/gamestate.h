#pragma once
#include <SFML\Graphics.hpp>
#include <SFML\Audio.hpp>
#include <iostream>
#include "wunsz.h"

class Game;
class GameState
{
public:
	GameState(Game *p, sf::RenderWindow *w);
	virtual void draw() = 0;
	virtual bool getInput(sf::Event &e) = 0;
	virtual void update() = 0;
	virtual void activate();
	virtual void deactivate();
	virtual ~GameState();
protected:
	Game *parent;
	sf::RenderWindow *window;
private:
};

class Splash : public GameState
{
public:
	Splash(Game *p, sf::RenderWindow *w);
	void draw();
	bool getInput(sf::Event &e);
	void update();
	~Splash();
private:
	sf::Texture backgroundPic;
	sf::VertexArray background;
	sf::Clock endTimer;
	sf::RectangleShape shade;
	sf::Uint8 shadeColor;
	sf::Sound whoosh;
	sf::SoundBuffer whooshFile;
};

class PlayState : public GameState
{
public:
	PlayState(Game *p, sf::RenderWindow *w, int *diffSetting);
	void draw();
	bool getInput(sf::Event &e);
	void update();
	void activate();
	void deactivate();
	~PlayState();
private:
	Snake wonsz;
	sf::Texture bgPic;
	sf::VertexArray background;
	sf::Music bgMusic;
};

class MainMenu : public GameState
{
public:
	MainMenu(Game *p, sf::RenderWindow *w);
	void draw();
	bool getInput(sf::Event &e);
	void update();
	~MainMenu();
private:
	sf::Font menuFont;
	sf::Text title, start, difficultyText, *diffSetting, exit;
	sf::Texture backgroundPic;
	sf::Sprite background;
	int *difficulty, difficultyCount;

};

class GameOver : public GameState
{
public:
	GameOver(Game *p, sf::RenderWindow *w);
	void draw();
	bool getInput(sf::Event &e);
	void update();
	~GameOver();
private:
	sf::RectangleShape background;
	sf::Clock deadTimer;
	sf::Font textFont;
	sf::Text gameOverText;
};

class PauseScreen : public GameState
{
public:
	PauseScreen(Game *p, sf::RenderWindow *w);
	void draw();
	bool getInput(sf::Event &e);
	void update();
	~PauseScreen();
private:
	sf::RectangleShape background;
	sf::Font pauseFont;
	sf::Text msg;
};