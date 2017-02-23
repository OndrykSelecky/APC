#include <iostream>
#include "Point.h"
#include "Shape.h"
#include "Circle.h"
#include "Rect.h"
#include "Group.h"

int main(int argc, char **argv)
{
	
	Point p(5, 5);
	std::unique_ptr<Group> g(new Group());

	std::unique_ptr<Shape> c(new Circle(p, 10));
	std::unique_ptr<Rect> r(new Rect(Point(2, 2), Point(1, 2)));
	g->add(std::move(c));
	g->add(std::move(r));
	std::unique_ptr<Group> v(new Group());
	std::unique_ptr<Shape> c2(new Circle(p, 5));
	v->add(std::move(c2));
	v->add(std::move(g));

	std::unique_ptr<Group> g2(new Group());
	std::unique_ptr<Shape> c3(new Rect(Point(2, 3), Point(5, 2)));
	g2->add(std::move(c3));
	v->add(std::move(g2));
	v->flatten();
	std::cout << v->m_shapes.size() << std::endl;
	
	
	for (auto &i : v->m_shapes)
	{
		Rect bb = i->bound_box();
		std::cout << bb.get_upper_left().X() << " " << bb.get_upper_left().Y() << ", " << bb.get_lower_right().X() << " " << bb.get_lower_right().Y() << std::endl;
	}
	/*
	Rect bb = v->bound_box();
	std::cout << bb.get_upper_left().X() << " " << bb.get_upper_left().Y() << ", " << bb.get_lower_right().X() << " " << bb.get_lower_right().Y() << std::endl;
	*/
	Group g1 = *v.get();
	Rect bb = g1.bound_box();
	v->move(10, -10);

	for (auto &i : v->m_shapes)
	{
		Rect bb = i->bound_box();
		std::cout <<" v:" << bb.get_upper_left().X() << " " << bb.get_upper_left().Y() << ", " << bb.get_lower_right().X() << " " << bb.get_lower_right().Y() << std::endl;
	}

	for (auto &i : g1.m_shapes)
	{
		Rect bb = i->bound_box();
		std::cout <<"g1: "<< bb.get_upper_left().X() << " " << bb.get_upper_left().Y() << ", " << bb.get_lower_right().X() << " " << bb.get_lower_right().Y() << std::endl;
	}

	return 0;
}