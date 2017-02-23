#pragma once
class Rect;

class Shape
{
public:	
	virtual ~Shape() {};
	Shape() {};
	virtual Rect bound_box() const =0;
	virtual void move(double x_offset, double y_offset) =0;	
};
