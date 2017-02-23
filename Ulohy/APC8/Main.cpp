#include "SimpleJSON.h"
#include <fstream>
#include <iostream>
#include <string>

int main()
{
	std::ifstream ifs;
	std::ofstream ofs;

	ifs.open("example.txt");
	ofs.open("output.txt");
	try {
		SimpleJSON js(ifs);
		//js.print(std::cout);
		/*
		if (!js.add("Quote:", "Don't be lasagna!")) std::cout << "Already in\n";
		if (!js.add("Quote:", 10)) std::cout << "Already in\n";
		
		if (js.remove("age")) std::cout << "removed\n";
		js.write(ofs);*/

		SimpleJSON js2;
		js2 = std::move(js);
		std::string s = "age";
		if (js2.contains(s)) printf("d");
		/*js.print(std::cout);
		if (!js2.add("Quote:", "Don't be lasagna!")) std::cout << "Already in\n";
		js2.print(std::cout);*/

	}
	catch (const std::invalid_argument& e)
	{
		std::cout << e.what();
	}
	

	return 0;
}