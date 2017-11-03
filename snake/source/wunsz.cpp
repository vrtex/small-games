#include "wunsz.h"

Node::Node(sf::Vector2u pos) :
    position(pos)
{
}

Node::Node(sf::Texture *pic, sf::Vector2u pos) :
    position(pos)
{
    body.setTexture(*pic);
    blobSize = pic->getSize();
    body.setOrigin((float)blobSize.x / 2, (float)blobSize.y / 2);
    body.setPosition((float)blobSize.x * position.x, (float)blobSize.y * position.y);
}

void Node::setPicture(sf::Texture *pic)
{
    body.setTexture(*pic);
    blobSize = pic->getSize();
    body.setPosition((float)blobSize.x * position.x, (float)blobSize.y * position.y);
    body.setOrigin((float)blobSize.x / 2, (float)blobSize.y / 2);
}

sf::Vector2u Node::getPosition() const
{
    return position;
}

void Node::draw(sf::RenderWindow *w)
{
    w->draw(body);
}

bool Node::checkCollision(sf::Vector2u pos)
{
	return pos.x == position.x && pos.y == position.y;
}

Node::~Node()
{

}

//------------------------HEADNODE------------------------------------------------

HeadNode::HeadNode(sf::Vector2u pos, std::vector<Node> *t, sf::Vector2u wSize) : Node(pos)
{
	headPic[0].loadFromFile("Res/head.png");
	headPic[1].loadFromFile("Res/headOpen.png");
	headPic[2].loadFromFile("Res/headDead.png");

	tailPic[0].loadFromFile("Res/tail/tailH1.png");
	tailPic[1].loadFromFile("Res/tail/tailH2.png");
	tailPic[2].loadFromFile("Res/tail/tailV1.png");
	tailPic[3].loadFromFile("Res/tail/tailV2.png");
	tailPic[4].loadFromFile("Res/tail/tailRB.png");
	tailPic[5].loadFromFile("Res/tail/tailLB.png");
	tailPic[6].loadFromFile("Res/tail/tailLT.png");
	tailPic[7].loadFromFile("Res/tail/tailRT.png");
	tailPicsCount = 8;

	for(int i = 0; i < 3; ++i)
	{
		foodPic[i].loadFromFile("Res/food/food_" + std::to_string(i) + ".png");
	}

    tail = t;
    tail->emplace_back(&tailPic[0], sf::Vector2u(pos.x - 2, pos.y));
    tail->emplace_back(&tailPic[1], sf::Vector2u(pos.x - 1, pos.y));
    setPicture(&headPic[0]);
	rotation = 0;
	oldRotation = 0;
	pictureIndex = 0;

	gameBounds.x = wSize.x / blobSize.x;
	gameBounds.y = wSize.y / blobSize.y;

	spawnFood();

	score = 0;
	scoreFont.loadFromFile("Res/font.ttf");
	scoreHUD.setFont(scoreFont);
	scoreHUD.setString("Score: " + std::to_string(score));
	scoreHUD.setPosition(20, 10);

	eatSoundFile.loadFromFile("Res/beep.wav");
	eatSound.setBuffer(eatSoundFile);
	eatSound.setVolume(50);
	eatSound.setPitch(1.5);
	eatSound.setLoop(false);
}

void HeadNode::draw(sf::RenderWindow * w)
{
	Node::draw(w);
	food->draw(w);
	w->draw(scoreHUD);
}


bool HeadNode::move(int x, int y, int r)
{
	rotation = r;
	int picIndex, diff = oldRotation - rotation;
	//choose next segment
	if(diff == 2 || diff == 0 || diff == -2) //straight
	{
		if(rotation == 1 || rotation == 3)
			picIndex = 2 + rand() % 2;
		else
			picIndex = 0 + rand() % 2;
	}
	else
	{
		if(oldRotation == 0)
			if(rotation == 3) picIndex = 6;
			else picIndex = 5;
		else if(oldRotation == 1)
			if(rotation == 0) picIndex = 7;
			else picIndex = 6;
		else if(oldRotation == 2)
			if(rotation == 1) picIndex = 4;
			else picIndex = 7;
		else if(oldRotation == 3)
			if(rotation == 2) picIndex = 5;
			else picIndex = 4;
	}
	oldRotation = rotation;

	//rotate head
	body.setRotation(90.0f * r);
	if(r == 2) body.setScale(1, -1);
	else body.setScale(1, 1);

	if(food->checkCollision({getPosition().x + 2 * x, getPosition().y + 2 * y}))
		pictureIndex = 1;
	else
		pictureIndex = 0;
	setPicture(&headPic[pictureIndex]);

	//check food
	if(!food->checkCollision({getPosition().x + x, getPosition().y + y}))
	{	//don't eat
		
		tail->erase(tail->begin());
	}
	else
	{	//eat
		++score;
		scoreHUD.setString("Score: " + std::to_string(score));
		eatSound.play();
		delete food;
		spawnFood();
	}
    tail->emplace_back(&tailPic[picIndex], getPosition());
    position.x += x;
    position.y += y;
    body.setPosition((float)blobSize.x * position.x, (float)blobSize.y * position.y);
	if(getPosition().x <= 0 || getPosition().y <= 0 || getPosition().x >= gameBounds.x || getPosition().y >= gameBounds.y)
	{	//out of room
		setPicture(&headPic[2]);
		return false;
	}
	for(auto &t : *tail)
	{
		if(t.checkCollision(position))
		{	//tail collision
			setPicture(&headPic[2]);
			return false;
		}
	}
	return true;
}

void HeadNode::spawnFood()
{
	unsigned int x, y;
	bool check;
	do
	{
		x = rand() % (gameBounds.x - 1) + 1;
		y = rand() % (gameBounds.y - 1) + 1;
		check = false;
		if(checkCollision({x, y}))
		{
			check = true;
			continue;
		}
		for(auto &t : *tail)
		{
			if(t.checkCollision({x, y}))
			{
				check = true;
				break;
			}
		}
	} while(check);
	food = new Node(&foodPic[rand() % 3], {x, y});
}

HeadNode::~HeadNode()
{

}

//----------------------SNAKE----------------------------------------------------------

Snake::Snake(sf::Vector2u wSize, int *diff) :
    head(sf::Vector2u(3, 2), &tail, wSize),
	windowSize(wSize), difficulty(400 - *diff * 40)
{
	delete diff;
    newDirection = currentDirection = {1, 0, 0};
    moveTimer.restart();
}

bool Snake::getInput(sf::Event & e)
{
	if(e.type == sf::Event::KeyPressed)
	{
		switch(e.key.code)
		{
		case sf::Keyboard::Up:
			if(currentDirection.y == 1) break;
			newDirection = {0, -1, 3};
			return true;
		case sf::Keyboard::Right:
			if(currentDirection.x == -1) break;
			newDirection = {1, 0, 0};
			return true;
		case sf::Keyboard::Down:
			if(currentDirection.y == -1) break;
			newDirection = {0, 1, 1};
			return true;
		case sf::Keyboard::Left:
			if(currentDirection.x == 1) break;
			newDirection = {-1, 0, 2};
			return true;
		default:
			break;
		}
	}
	return false;
}

bool Snake::update()
{
    if(moveTimer.getElapsedTime().asMilliseconds() > difficulty)
    {
		if(!head.move(newDirection.x, newDirection.y, newDirection.z)) return false;
		currentDirection = newDirection;
        moveTimer.restart();
    }
	return true;
}

void Snake::draw(sf::RenderWindow *w)
{
    for(auto &t : tail)
    {
        t.draw(w);
    }
    head.draw(w);
}

Snake::~Snake()
{

}
