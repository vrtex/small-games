#include "gamestate.h"
#include "game.h"


GameState::GameState(Game *p, sf::RenderWindow *w) :
	parent(p), window(w)
{
}


void GameState::draw()
{
}

void GameState::update()
{
}

void GameState::activate()
{
}

void GameState::deactivate()
{
}

GameState::~GameState()
{
	std::cout << "delete genereal\n";
}

//----------------------PLAYSTATE----------------

PlayState::PlayState(Game *p, sf::RenderWindow *w, int *diffSetting) :
	GameState(p, w), wonsz(w->getSize(), diffSetting)
{
	bgPic.loadFromFile("Res/bgPic.png");
	background.setPrimitiveType(sf::Quads);
	background.resize(4);
	background[0].position = {0, 0};
	background[1].position = {(float)w->getSize().x, 0};
	background[2].position = {(float)w->getSize().x, (float)w->getSize().y};
	background[3].position = {0, (float)w->getSize().y};
	background[0].texCoords = {0, 0};
	background[1].texCoords = {(float)bgPic.getSize().x, 0};
	background[2].texCoords = {(float)bgPic.getSize().x, (float)bgPic.getSize().y};
	background[3].texCoords = {0, (float)bgPic.getSize().y};

	bgMusic.openFromFile("Res/bgMusic.ogg");
	bgMusic.setVolume(10);
	bgMusic.setLoop(true);
	bgMusic.play();
}

void PlayState::draw()
{
	window->draw(background, &bgPic);
	wonsz.draw(window);
}

bool PlayState::getInput(sf::Event & e)
{
	if(e.type == sf::Event::KeyPressed)
	{
		if(e.key.code == sf::Keyboard::Escape)
		{
			parent->addRequest(new PauseScreen(parent, window));
			return true;
		}
	}
	if(wonsz.getInput(e)) return true;
	return false;
}

void PlayState::update()
{
	if(!wonsz.update())
	{
		deactivate();
		parent->addRequest(new GameOver(parent, window));
	}
}

void PlayState::activate()
{
	bgMusic.play();
}

void PlayState::deactivate()
{
	bgMusic.pause();
}

PlayState::~PlayState()
{
	std::cout << "delete play\n";
	bgMusic.stop();
}

//--------------------SPLASHES--------------

Splash::Splash(Game *p, sf::RenderWindow *w) :
	GameState(p, w)
{
	backgroundPic.loadFromFile("Res/splash.png");
	background.setPrimitiveType(sf::Quads);
	background.resize(4);
	background[0].texCoords = {0, 0};
	background[1].texCoords = {(float)backgroundPic.getSize().x, 0};
	background[2].texCoords = {(float)backgroundPic.getSize().x, (float)backgroundPic.getSize().y};
	background[3].texCoords = {0, (float)backgroundPic.getSize().y};
	background[0].position = {0, 0};
	background[1].position = {(float)w->getSize().x, 0};
	background[2].position = {(float)w->getSize().x, (float)w->getSize().y};
	background[3].position = {0, (float)w->getSize().y};

	shade.setSize((sf::Vector2f) w->getSize());
	shade.setFillColor(sf::Color(0, 0, 0, 0));
	shade.setPosition(0, 0);
	shadeColor = 0;

	whooshFile.loadFromFile("Res/whoosh.wav");
	whoosh.setBuffer(whooshFile);
	whoosh.play();
	endTimer.restart();
}

void Splash::draw()
{
	window->draw(background, &backgroundPic);
	window->draw(shade);
}

bool Splash::getInput(sf::Event & e)
{
	return false;
}

void Splash::update()
{
	if(endTimer.getElapsedTime().asMilliseconds() > 1500 - 256)
	{

		if(shadeColor < 200)
			shadeColor += 16;
		else shadeColor = 255;
		shade.setFillColor(sf::Color(0, 0, 0, shadeColor));
	}
	if(endTimer.getElapsedTime().asMilliseconds() > 1500)
		parent->changeRequest(new MainMenu(parent, window));
}

Splash::~Splash()
{
}


//---------------------MAINMENU--------------------

MainMenu::MainMenu(Game * p, sf::RenderWindow * w) :
	GameState(p, w)
{
	difficulty = new int;
	*difficulty = 1;
	difficultyCount = 5;
	diffSetting = new sf::Text[difficultyCount];

	backgroundPic.loadFromFile("Res/mainmenu.png");
	background.setTexture(backgroundPic);
	int width = backgroundPic.getSize().x / 2;
	float xPos = (float)w->getSize().x / 2 - width;
	background.setPosition(xPos, 0);

	menuFont.loadFromFile("Res/font.ttf");
	title.setFont(menuFont);
	start.setFont(menuFont);
	difficultyText.setFont(menuFont);
	exit.setFont(menuFont);

	title.setString("Generic Snake");
	start.setString("Start Game");
	difficultyText.setString("Choose difficulty:");
	exit.setString("Exit game");


	title.setCharacterSize(50);
	width = title.getGlobalBounds().width / 2;
	xPos = w->getSize().x / 2 - width;
	title.setPosition(xPos, 100);

	width = start.getGlobalBounds().width / 2;
	xPos = w->getSize().x / 2 - width;
	start.setPosition(xPos, 300);

	width = difficultyText.getGlobalBounds().width / 2;
	xPos = w->getSize().x / 2 - width;
	difficultyText.setPosition(xPos, 350);

	for(int i = 0; i < difficultyCount; ++i)
	{
		diffSetting[i].setFont(menuFont);
		diffSetting[i].setPosition(xPos + i * 50, 400);
		diffSetting[i].setString(std::to_string(i + 1));
	}
	/*
	diffSetting[0].setFont(menuFont);
	diffSetting[1].setFont(menuFont);
	diffSetting[2].setFont(menuFont);
	diffSetting[0].setString("1");
	diffSetting[1].setString("2");
	diffSetting[2].setString("3");
	diffSetting[0].setPosition(xPos + 50, 400);
	diffSetting[1].setPosition(xPos + 100, 400);
	diffSetting[2].setPosition(xPos + 150, 400);
	*/

	diffSetting[0].setFillColor(sf::Color::Cyan);

	width = exit.getGlobalBounds().width / 2;
	xPos = w->getSize().x / 2 - width;
	exit.setPosition(xPos, 450);
}

void MainMenu::draw()
{
	window->draw(background);
	window->draw(title);
	window->draw(start);
	window->draw(difficultyText);
	for(int i = 0; i < difficultyCount; ++i)
	{
		window->draw(diffSetting[i]);
	}
	window->draw(exit);
}

bool MainMenu::getInput(sf::Event & e)
{
	if(e.type == sf::Event::MouseButtonPressed)
	{
		int mx = e.mouseButton.x, my = e.mouseButton.y;
		if(mx >= start.getGlobalBounds().left &&
			my >= start.getGlobalBounds().top &&
			mx <= start.getGlobalBounds().left + start.getGlobalBounds().width &&
			my <= start.getGlobalBounds().top + start.getGlobalBounds().height)
		{
			parent->changeRequest(new PlayState(parent, window, difficulty));
			return true;
		}
		for(int i = 0; i < difficultyCount; ++i)
		{
			if(mx >= diffSetting[i].getGlobalBounds().left &&
				my >= diffSetting[i].getGlobalBounds().top &&
				mx <= diffSetting[i].getGlobalBounds().left + diffSetting[i].getGlobalBounds().width &&
				my <= diffSetting[i].getGlobalBounds().top + diffSetting[i].getGlobalBounds().height)
			{
				for(int j = 0; j < difficultyCount; ++j)
				{
					diffSetting[j].setFillColor(sf::Color::White);
				}
				diffSetting[i].setFillColor(sf::Color::Cyan);
				*difficulty = i + 1;
				return true;
			}
		}
		if(mx >= exit.getGlobalBounds().left &&
			my >= exit.getGlobalBounds().top &&
			mx <= exit.getGlobalBounds().left + exit.getGlobalBounds().width &&
			my <= exit.getGlobalBounds().top + exit.getGlobalBounds().height)
		{
			window->close();
			return true;
		}
	}
	return false;
}

void MainMenu::update()
{
}

MainMenu::~MainMenu()
{
	delete[] diffSetting;
}

//-----------------GAMEOVER---------------------

GameOver::GameOver(Game * p, sf::RenderWindow * w) :
	GameState(p, w), background((sf::Vector2f)w->getSize())
{
	background.setFillColor(sf::Color(51, 50, 31, 122));
	textFont.loadFromFile("Res/font.ttf");
	gameOverText.setString("Game over");
	gameOverText.setFont(textFont);
	gameOverText.setPosition(w->getSize().x / 2 - gameOverText.getGlobalBounds().left / 2, 200);

	deadTimer.restart();
}

void GameOver::draw()
{
	window->draw(background);
	window->draw(gameOverText);
}

bool GameOver::getInput(sf::Event & e)
{
	return false;
}

void GameOver::update()
{
	if(deadTimer.getElapsedTime().asSeconds() > 2)
	{
		parent->deleteRequest(2);
		parent->addRequest(new MainMenu(parent, window));
	}
}

GameOver::~GameOver()
{
}


//------------------------PAUSESCREEN---------------------------

PauseScreen::PauseScreen(Game *p, sf::RenderWindow *w) :
	GameState(p, w)
{
	pauseFont.loadFromFile("Res/font.ttf");
	msg.setString("Paused\n press space or enter to continue\n press escape to exit");
	msg.setFont(pauseFont);
	msg.setPosition(w->getSize().x / 2 - msg.getGlobalBounds().width / 2, w->getSize().y / 2 - msg.getGlobalBounds().height / 2);

	background.setSize((sf::Vector2f)w->getSize());
	background.setPosition(0, 0);
	background.setFillColor(sf::Color(98, 155, 23, 125));
}

void PauseScreen::draw()
{
	window->draw(background);
	window->draw(msg);
}

bool PauseScreen::getInput(sf::Event & e)
{
	if(e.type == sf::Event::KeyPressed)
	{
		if(e.key.code == sf::Keyboard::Space || e.key.code == sf::Keyboard::Return)
		{
			parent->deleteRequest(1);
			return true;
		}
		if(e.key.code == sf::Keyboard::Escape)
		{
			parent->deleteRequest(2);
			parent->addRequest(new MainMenu(parent, window));
			return true;
		}
	}
	return false;
}

void PauseScreen::update()
{
}

PauseScreen::~PauseScreen()
{
}
