#pragma once

#ifndef VECTOR2_H
#define VECTOR2_H

class Vector2 {
public:
	Vector2() {};
	~Vector2() {};

	Vector2(double x1, double y1)
		:x(x1), y(y1)
	{};
	void set(double x1, double y1)
	{
		x = x1;
		y = y1;
	}
	

	Vector2 operator - (const Vector2 v) const
	{
		return Vector2(x - v.x, y - v.y);
	}

	Vector2 operator + (const Vector2 v) const
	{
		return Vector2(x + v.x, y + v.y);
	}

	Vector2 operator * (const double k) const
	{
		return Vector2(x * k, y * k);
	}


	double dot(const Vector2 v) const
	{
		return x*v.x + y*v.y;
	}

	Vector2 Vertical() const
	{
		return Vector2(y, -x);
	}

private:
	double x, y;
};

#endif // !Vector2_H