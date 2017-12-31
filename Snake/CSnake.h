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

class Snake : public CObject
{
	DECLARE_SERIAL(Snake);

private:
	UINT direction;
	BOOL is_food;

public:
	vector<CPoint> snake_list;
	vector<CPoint> default_state;
	CPoint food;
	UINT speed;

public:
	Snake();
	virtual ~Snake();
	virtual void Serialize(CArchive& ar);

public:
	BOOL move();
	BOOL isDied();
	void generateFood();
	void changeDirection(UINT);
	UINT getDirection();
	BOOL isFood();
	void setFood(BOOL);

};

