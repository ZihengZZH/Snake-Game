#pragma once
#include "afx.h"

#include <vector>
#include <iterator>

using namespace std;

#define UP 1
#define DOWN 2
#define LEFT 3
#define RIGHT 4

#define BORDER 60
#define HEIGHT 600
#define WIDTH 600

// Snake class to manipulate the snake object
class Snake : public CObject
{
	DECLARE_SERIAL(Snake);

private:
	

public:
	vector<CPoint> snake_list; // snake body
	vector<CPoint> default_state; // default state
	UINT direction; // direction of the snake
	BOOL is_food; // if the snake eats the food	
	CPoint food; // food
	UINT speed; // speed of the snake
	UINT level; // level of the game
	BOOL level_up; // if level up

public:
	Snake();
	virtual ~Snake();
	virtual void Serialize(CArchive& ar);

public:
	BOOL move(); // Function to move the snake and determine if died
	BOOL isDied(); // Function to return if the snake is died
	void generateFood(); // Function to generate the food randomly
	void changeDirection(UINT); // Function to change the direction
	UINT getDirection(); // Function to get the direction
	BOOL isFood(); // Function to return the attribute
	void setFood(BOOL); // Function to set if the snake eats the food

};


/*
The LEVEL is increasing as the score goes up, 
which means some scores indicate one level and so do others.
However, the intervals between adjacent levels (scores) are different.
And the interval of scores also increases as 10, 20, 30, 40, 50, 60 etc.
So the pivots of levels are as follows:
	10	20	40	70	110  160  ......
*/
