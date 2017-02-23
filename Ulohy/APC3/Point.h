#pragma once

class Point
{
public:
	Point() {};
	Point(double x, double y);
	Point(const Point &other);
	Point& operator=(const Point& rhs);
	double X() const;
	double Y() const;
private:
	double m_x;
	double m_y;
};
