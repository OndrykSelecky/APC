#pragma once
#include "Shape.h"
#include "Point.h"


class Rect : public Shape
{
public:
	
	Rect() {};
	Rect(Point a, Point b);
	Rect(const Rect &other);
	Rect& operator=(const Rect& rhs);
	virtual Rect bound_box() const override;
	virtual void move(double x_offset, double y_offset);
	Point get_upper_left() const;
	Point get_lower_right() const;

private:
	Point m_a;
	Point m_b;
};
