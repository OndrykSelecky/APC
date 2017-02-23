#include "Rect.h"
#include <iostream>
#include <algorithm>

Rect::Rect(Point a, Point b)
{
	/*
	takes coordinates of upper left and lower right corner
	*/
	m_a = Point(std::min(a.X(), b.X()), std::max(a.Y(), b.Y()));
	m_b = Point(std::max(a.X(), b.X()), std::min(a.Y(), b.Y()));
}

Rect::Rect(const Rect & other)
{
	m_a = other.m_a;
	m_b = other.m_b;	
}

Rect & Rect::operator=(const Rect & rhs)
{	
	return *this;
}

Rect Rect::bound_box() const
{
	
	return *this;
}

void Rect::move(double x_offset, double y_offset)
{	
	m_a = Point(m_a.X() + x_offset, m_a.Y() + y_offset);
	m_b = Point(m_b.X() + x_offset, m_b.Y() + y_offset);	
}

Point Rect::get_upper_left() const
{
	return m_a;
}

Point Rect::get_lower_right() const
{
	return m_b;
}


