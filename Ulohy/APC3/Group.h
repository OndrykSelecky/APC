#pragma once

#include "Shape.h"
#include "Rect.h"
#include <vector>
#include <memory>

class Group : public Shape
{
public:
	Group() {};	
	Group(const Group &other);
	Group& operator=(const Group& rhs);
	void flatten();
	void add(std::unique_ptr<Shape> s);
	virtual Rect bound_box() const override;
	virtual void move(double x_offset, double y_offset);	

private:
	std::vector<std::unique_ptr<Shape>> m_shapes;
};