#include <iostream>
#include <string>
#include "Sudoku.h"
#include <iomanip>

int main(int argc, char **argv) {
	//Sudoku s;
	std::string str = "3 0 6 5 0 8 4 0 0 \n5 2 0 0 0 0 0 0 0 \n0 8 7 0 0 0 0 3 1 \n0 0 3 0 1 0 0 8 0 \n9 0 0 8 6 3 0 0 5 \n0 5 0 0 9 0 6 0 0 \n1 3 0 0 0 0 2 5 0 \n0 0 0 0 0 0 0 7 4 \n0 0 5 2 0 6 3 0 0 \n";
	
	Sudoku s2(str);
	std::cout << s2 << std::endl;
	/*
	++s2;
	++s2;
	++s2;
	std::cout << s2 << std::endl;
	std::string str;
	
	std::cout << s << std::endl;*/
	++s2;
	std::cout << s2 << std::endl;
	s2.solve();
	std::cout << s2 << std::endl;
	
	if (s2.is_valid()) std::cout << "valid\n";

	return 0;
}