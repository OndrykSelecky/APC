#include "Group.h"
#include "Circle.h"
#include <iostream>
#include <limits>

void Group::flatten()
{
	/*
	New vector of Shape, all circles and rects from hierarchy are moved in this flattened vector
	*/
	std::vector<std::unique_ptr<Shape>> new_shapes;
	for (size_t i = 0;i<m_shapes.size();i++)	//cez referenciu
	{
		Group* g = dynamic_cast<Group*>(m_shapes[i].get());
		if (g != nullptr)
		{
			//if element is Group, flatten and move all elements into new vector
			g->flatten();
			for (auto &s : g->m_shapes)
			{
				new_shapes.push_back(std::move(s));
			}			
		}
		else
		{
			new_shapes.push_back(std::move(m_shapes[i]));
		}
	}

	/*
	* original vector is cleared and new is copied in original
	*/
	m_shapes.clear();
	for (size_t i = 0; i < new_shapes.size(); i++)
	{
		m_shapes.push_back(std::move(new_shapes[i]));
	}
	
}

void Group::add(std::unique_ptr<Shape> s)
{
	m_shapes.push_back(std::move(s));
}

Rect Group::bound_box() const
{
	if (m_shapes.size() == 0)
	{
		return Rect(Point(0, 0), Point(0, 0));
	}

	//stores all bounding boxes af shapes in group in array bounding_rects
	std::vector<Rect> bounding_rects;
	for (size_t i = 0; i<m_shapes.size(); i++)
	{
		bounding_rects.push_back(m_shapes[i]->bound_box());
	}

	/*
	Computes bounding box.
	coordinates are [min x, max y], [max x, min y] from array of bounding rectangles
	*/
	Point upper_left(std::numeric_limits<double>::max(), std::numeric_limits<double>::min());
	Point lower_right(std::numeric_limits<double>::min(), std::numeric_limits<double>::max());
	for (auto rect : bounding_rects)
	{
		if (rect.get_upper_left().X() < upper_left.X())
		{
			upper_left = Point(rect.get_upper_left().X(), upper_left.Y());
		}
		if (rect.get_upper_left().Y() > upper_left.Y())
		{
			upper_left = Point(upper_left.X(), rect.get_upper_left().Y());
		}
		if (rect.get_lower_right().X() > lower_right.X())
		{
			lower_right = Point(rect.get_lower_right().X(), lower_right.Y());
		}
		if (rect.get_lower_right().Y() < lower_right.Y())
		{
			lower_right = Point(lower_right.X(), rect.get_lower_right().Y());
		}
	}

	return Rect(upper_left, lower_right);
}

void Group::move(double x_offset, double y_offset)
{
	
	for (size_t i = 0; i<m_shapes.size(); i++)
	{
		m_shapes[i]->move(x_offset, y_offset);
	}

}


Group::Group(const Group & other)
{	
	*this = other;
}


Group & Group::operator=(const Group & rhs)
{
	
	for (auto& s : rhs.m_shapes)
	{
		//I had problem with dynamic cast, where creating object of abstract class Shape was not allowed

		Group* g = dynamic_cast<Group*>(s.get());
		if (g != nullptr)
		{
			m_shapes.push_back(std::unique_ptr<Shape>(new Group(*g)));
			continue;
		}
		Rect* r = dynamic_cast<Rect*>(s.get());
		if (r != nullptr)
		{
			m_shapes.push_back(std::unique_ptr<Shape>(new Rect(*r)));
			continue;
		}
		Circle* c = dynamic_cast<Circle*>(s.get());
		if (c != nullptr)
		{
			m_shapes.push_back(std::unique_ptr<Shape>(new Circle(*c)));
			continue;
		}
	}

	return *this;
}
