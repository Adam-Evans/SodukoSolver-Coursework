#include "Vector2.h"



Vector2::Vector2()
{
	x = 0;
	y = 0;
}

Vector2::Vector2(const int _x, const int _y)
{
	x = _x;
	y = _y;
}


Vector2::~Vector2()
{
}

void Vector2::setX(const int _x)
{
	x = _x;
}

int Vector2::getX() const
{
	return x;
}

void Vector2::setY(const int _y)
{
	y = _y;
}

int Vector2::getY() const
{
	return y;
}
