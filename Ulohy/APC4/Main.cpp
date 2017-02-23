
#include "BigNum.h"
#include <iostream>
#include <cctype>
#include <string>

int main(int argc, char **argv)
{
	
	BigNum a("+98767262874645634329869797");
	BigNum b(0);
	BigNum c(-2);
	a += b;
	std::cout << a.to_string() << std::endl;


	BigNum sum = b + c;
	std::cout << sum.to_string() << std::endl;

	BigNum sum2 = b + a;
	std::cout << sum2.to_string() << std::endl;
	
	/*BigNum c(-123456789);
	BigNum d(-123456788);
	if (c == d) std::cout << "c == d\n";
	if (c != d) std::cout << "c != d\n";
	if (c > d) std::cout << "c > d\n";
	if (d > c) std::cout << "d > c\n";
	if (c < d) std::cout << "c < d\n";
	if (d < c) std::cout << "d < c\n";
	if (c >= d) std::cout << "c >= d\n";
	if (d >= c) std::cout << "d >= c\n";
	if (c <= d) std::cout << "c <= d\n";
	if (d <= c) std::cout << "d <= c\n";*/
	
	return 0;
}