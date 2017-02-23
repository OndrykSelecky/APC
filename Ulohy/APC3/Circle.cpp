#include "Circle.h"
#include <iostream>

Circle::Circle(Point c, double b)
{
	m_center = c;
	m_radius = b;		
}

Circle::Circle(const Circle & other)
{
	m_center = other.m_center;
	m_radius = other.m_radius;	
}

Circle & Circle::operator=(const Circle & rhs)
{	
	return *this;
}

Rect Circle::bound_box() const 
{
	Point upper_left(m_center.X() - m_radius, m_center.Y() + m_radius);
	Point lower_right(m_center.X() + m_radius, m_center.Y() - m_radius);

	return Rect(upper_left, lower_right);
}

void Circle::move(double x_offset, double y_offset)
{			
	m_center = Point(m_center.X() + x_offset, m_center.Y() + y_offset);
}
