#include "stdafx.h"
#include "CSnake.h"


// The original length of the snake should be 3
// This should be fixed later on
Snake::Snake()
{
	CPoint point;
	point = CPoint(310, 310);
	snake_list.push_back(point);
	//point = CPoint(330, 310);
	//snake_list.push_back(point);
	//point = CPoint(350, 310);
	//snake_list.push_back(point);

	default_state = snake_list;

	direction = RIGHT;
	is_food = FALSE;
}


Snake::~Snake()
{
}

BOOL Snake::move()
{
	CPoint point;
	point = snake_list.front();

	if (getDirection() == UP)
		point.y -= 20;
	else if (getDirection() == DOWN)
		point.y += 20;
	else if (getDirection() == LEFT)
		point.x -= 20;
	else
		point.x += 20;

	if (point == food)
	{
		snake_list.push_back(*(snake_list.end() - 1));
		for (vector<CPoint>::iterator iter = snake_list.end() - 2;
			iter != snake_list.begin(); iter--)
		{
			iter->x = (iter - 1)->x;
			iter->y = (iter - 1)->y;
		}
		snake_list.begin()->x = point.x;
		snake_list.begin()->y = point.y;
		generateFood();
	}
	else
	{
		for (vector<CPoint>::iterator iter = snake_list.end() - 1;
			iter != snake_list.begin(); iter--)
		{
			iter->x = (iter - 1)->x;
			iter->y = (iter - 1)->y;
		}
		snake_list.begin()->x = point.x;
		snake_list.begin()->y = point.y;
	}

	if (snake_list.front().x < BORDER || snake_list.front().x>WIDTH + BORDER ||
		snake_list.front().y < BORDER || snake_list.front().y>HEIGHT + BORDER)
	{
		return FALSE;
	}
	for (vector<CPoint>::iterator iter = snake_list.begin() + 1;
		iter != snake_list.end(); ++iter) {
		if (snake_list.front().x == iter->x
			&& snake_list.front().y == iter->y) {
			return FALSE;
		}
	}

	return TRUE;
}

BOOL Snake::isDied()
{
	return FALSE;
}

void Snake::generateFood()
{
	UINT x = rand() % (WIDTH - 20);
	UINT y = rand() % (HEIGHT - 20);
	UINT X = 20 * (x / 20) + 10 + BORDER;
	UINT Y = 20 * (y / 20) + 10 + BORDER;
	CPoint point = CPoint(X, Y);
	food = CPoint(point);
}

void Snake::changeDirection(UINT ndir)
{
	direction = ndir;
}

UINT Snake::getDirection()
{
	return direction;
}

BOOL Snake::isFood()
{
	return is_food;
}

void Snake::setFood(BOOL n)
{
	is_food = n;
}
