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
	UINT direction; // direction of the snake
	BOOL is_food; // if the snake eats the food

public:
	vector<CPoint> snake_list; // snake body
	vector<CPoint> default_state; // default state
	CPoint food; // food
	UINT speed; // speed of the snake

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

