#pragma once
#include <deque>
#ifndef uint
typedef unsigned int uint;
#endif // !uint
struct Element {
	int x;
	int y;
};
enum Direction {
	Up,Down,Left,Right
};
enum DirectionChange {
	Clockwise,CounterClockWise
};
class Snake
{
public:
	Snake(int Startx, int Starty, Direction StartDirection,int StartLength);
	~Snake();
	int arrayStart;
	std::deque<Element> Elements;
	
	void eat(uint nutrition);
	void changeDirection(DirectionChange directionChange);
	Direction getDirection();
	void move();

	/// <summary>
	/// Checks if the Snake exists at a point
	///	</summary>
	/// <param name="x"></param>
	/// <param name="y"></param>
	/// <param name="headOnly">Only Checking Head if true - Needed for Eating</param>
	/// <returns></returns>
	bool collisionCheck(int x, int y, bool headonly);
	bool selfCollisioncheck();
	inline uint getNutrition() {return nutrition;};
	
	/// <summary>
	/// Checks if Snake has hit a Wall after Moving
	/// </summary>
	/// <param name="width"> Width of the Playingfield </param>
	/// <param name="height">Height of the Playingfield </param>
	/// <returns></returns>
	bool hasHitWall(int width, int height);
	
private:
	Direction currentDirection;
	int maxSize;
	uint nutrition=0;
};

