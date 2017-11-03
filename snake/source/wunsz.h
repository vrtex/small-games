#ifndef WUNSZ_H
#define WUNSZ_H

#include <SFML\Graphics.hpp>
#include <SFML\Audio.hpp>
#include <iostream>
#include <vector>

//----------------NODE-----------------

class Node
{
public:
    Node(sf::Vector2u pos);
    Node(sf::Texture *pic, sf::Vector2u pos);
    void setPicture(sf::Texture *pic);
    sf::Vector2u getPosition() const;
    void draw(sf::RenderWindow *w);
	bool checkCollision(sf::Vector2u pos);
    ~Node();
protected:
    sf::Vector2u position;
    sf::Vector2u blobSize;
    sf::Sprite body;
};

//--------------------HEADNODE------------------

class HeadNode : public Node
{
public:
    HeadNode(sf::Vector2u pos, std::vector<Node> *t, sf::Vector2u wSize);
	void draw(sf::RenderWindow *w);
	bool move(int x, int y, int r);
	void spawnFood();
    ~HeadNode();
private:
    sf::Texture headPic[3], tailPic[8], foodPic[3];
	int tailPicsCount;
    std::vector<Node> *tail;
	sf::Vector2u gameBounds;
	int rotation;
	int oldRotation;
	int pictureIndex;
	Node *food;

	int score;
	sf::Font scoreFont;
	sf::Text scoreHUD;

	sf::Sound eatSound;
	sf::SoundBuffer eatSoundFile;
};

//------------------SNAKE------------------------

class Snake
{
public:
    Snake(sf::Vector2u wSize, int *diff);
	bool getInput(sf::Event &e);
    bool update();
    void draw(sf::RenderWindow *w);
    ~Snake();
private:
    sf::Clock moveTimer;
	int difficulty;
	sf::Vector3i currentDirection, newDirection;
    std::vector<Node> tail;
    HeadNode head;
	sf::Vector2u windowSize;
};

#endif /* end of include guard: WUNSZ_H */
