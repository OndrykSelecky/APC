#include <iostream>
#include "foo.h"
#include <memory>

int main(int argc, char **argv) {
	Foo();//ked nema meno, vola sa hned konstruktor aj destruktor
	
	std::unique_ptr<Foo> p(new Foo()); 
	auto p2 = std::make_unique<Foo>(); //lepsi, c++14

	std::unique_ptr<std::string[]> s(new std::string[42]{ "ahoj" }); //pole stringov

	

	return 0;
}

//visual studio code
//iterovanie objektov - for(auto pair: ..) radsej for (const auto& pair : ..) - nemodifikuje
//numeric_limits<t>::max() - lepsie na zistovanie maxima
//nazvy funkcie vs navratove typy
//co je size_t
//#pragma once
//velkym pismenom na 
//using nie v .h suboroch