#pragma once
class Vector2
{
public:
	Vector2();
	Vector2(const int _x, const int _y);
	~Vector2();
	
	void setX(const int _x );
	int getX() const;
	void setY(const int _y );
	int getY() const;

private:
	int x;
	int y;
};

