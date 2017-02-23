#include "Point.h"
#include <iostream>

	
Point::Point(double x, double y)
{	
	this->m_x = x;
	this->m_y = y;
}

Point::Point(const Point &other)
{	
	m_x = other.m_x;
	m_y = other.m_y;	
}

Point& Point::operator=(const Point& rhs)
{	
	this->m_x = rhs.m_x;
	this->m_y = rhs.m_y;
	return *this;
}

double Point::X() const
{
	return this->m_x;
}

double Point::Y() const
{
	return this->m_y;
}

