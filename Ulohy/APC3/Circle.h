#pragma once

#include "Shape.h"
#include "Point.h"
#include "Rect.h"

class Circle : public Shape
{
public:
	Circle() {};
	Circle(Point c, double b);
	Circle(const Circle &other);
	Circle& operator=(const Circle& rhs);
	virtual Rect bound_box() const override;
	virtual void move(double x_offset, double y_offset);

private:
	Point m_center;
	double m_radius;
};
