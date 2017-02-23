#include "foo.h"
#include <cstdint>
#include <utility>
#include <iostream>

Foo::Foo()
	: m_i(125) //pozri
	, m_s("My string");
{
	std::cout << "Foo" << std::endl;

}

Foo::~Foo()
{
	std::cout << "~Foo" << std::endl;
}
