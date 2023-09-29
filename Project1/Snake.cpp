#include "Snake.hpp"

Snake::Snake(int Startx, int Starty,Direction StartDirection, int StartLength)
{
	nutrition = StartLength - 1;
	currentDirection = StartDirection;
	Elements.push_front({ Startx,Starty });
	for (int i = 1; i < StartLength; i++) { // start with one, since we created the first before starting
		move(); // Creates a new element on next position, since we have eaten, it won't delete the previous one
	}

}

Snake::~Snake()
{
}

void Snake::eat(uint nutrition)
{
	this->nutrition+=nutrition;
}

void Snake::changeDirection(DirectionChange directionChange)
{
	switch (currentDirection)
	{
	case Up:
		if (directionChange == Clockwise) {
			currentDirection = Right;
		}
		else
		{
			currentDirection = Left;
		}
		break;
	case Down:
		if (directionChange == Clockwise) {
			currentDirection = Left;
		}
		else
		{
			currentDirection = Right;
		}
		break;
	case Left:
		if (directionChange == Clockwise) {
			currentDirection = Up;
		}
		else
		{
			currentDirection = Down;
		}
		break;
	case Right:
		if (directionChange == Clockwise) {
			currentDirection = Down;
		}
		else
		{
			currentDirection = Up;
		}
		break;
	default:
		break;
	}
}

Direction Snake::getDirection()
{
    return currentDirection;
}

void Snake::move()
{
	int x = Elements.front().x;
	int y = Elements.front().y;
	switch (currentDirection)
	{
	case Up:
		--y;
		break;
	case Down:
		++y;
		break;
	case Left:
		--x;
		break;
	case Right:
		++x;
		break;
	default:
		break;
	}
	Element newElement = { x,y };
	Elements.push_front(newElement);
	if (nutrition==0) {
		Elements.pop_back();
	}
	else {
		--nutrition;
	}
}

bool Snake::collisionCheck(int x, int y, bool headOnly)
{
	if(!headOnly)
	{
		for (Element e : Elements)
		{
			if (e.x == x && e.y == y) {
				return true;
			}
		}
		return false;
	}
	return Elements[0].x == x && Elements[0].y == y;
}

bool Snake::selfCollisioncheck()
{
	for (int i = 1; i < Elements.size(); i++) {
		if (collisionCheck(Elements[i].x, Elements[i].y, true)) {
			return true;
		}
	}
	return false;
}

bool Snake::hasHitWall(int width, int height)
{
	if (Elements[0].x < 0 || Elements[0].y < 0 || Elements[0].x >= width || Elements[0].y >= height) {
		return true;
	}
	return false;
}


