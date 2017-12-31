#include "stdafx.h"
#include "CSnake.h"

IMPLEMENT_SERIAL(Snake, CObject, 1)

// The original length of the snake is 3
Snake::Snake()
{
	CPoint point;
	point = CPoint(310, 310);
	snake_list.push_back(point);
	snake_list.push_back(*(snake_list.end() - 1));
	snake_list.push_back(*(snake_list.end() - 1));
	
	food = point;
	default_state = snake_list;
	direction = RIGHT;
	is_food = FALSE;
	speed = 150;
}


Snake::~Snake()
{
}

void Snake::Serialize(CArchive & ar)
{
	if (ar.IsLoading())
	{
		int size;
		ar >> size;
		CPoint point;
		for (int i = 0; i < size; i++)
		{
			ar >> point;
			snake_list.push_back(point);
		}
		ar >> food;
		ar >> direction;
		ar >> is_food;
		ar >> speed;
	}
	else if (ar.IsStoring())
	{
		int size = snake_list.size();
		ar << size;
		for (auto snake_lst : snake_list)
		{
			ar << snake_lst;
		}
		ar << food;
		ar << direction;
		ar << is_food;
		ar << speed;
	}
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
		iter != snake_list.end(); ++iter) 
	{
		if (snake_list.front().x == iter->x
			&& snake_list.front().y == iter->y) 
		{
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
	BOOL overlap = TRUE;
	UINT x, y, X, Y;
	CPoint point;
	while (overlap)
	{
		overlap = FALSE;
		x = rand() % (WIDTH - 20);
		y = rand() % (HEIGHT - 20);
		X = 20 * (x / 20) + 10 + BORDER;
		Y = 20 * (y / 20) + 10 + BORDER;
		for (vector<CPoint>::iterator iter = snake_list.begin();
			iter != snake_list.end(); iter++)
		{
			point = CPoint(X, Y);
			if (*iter == point)
			{
				overlap = TRUE;
				break;
			}	
			else if (iter == snake_list.end() - 1)
			{
				overlap = FALSE;
				break;
			}
		}
	}
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
