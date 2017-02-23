#include <iostream>
#include <string>
#include "SecureString.h"
#include "SecureStringUnprotected.h"
#define n 50

int main(int argc, char **argv)
{


	SecureString ss("Prvy");

	ss.print();
	SecureString ss2("Druhy string\0");
	ss2.print();
	ss2.AppendChar('o');
	ss2.print();
	ss2.AppendChar('o');
	ss2.print();
	ss2.Clear();
	ss2.AppendChar('o');
	ss2.print();
	ss2.AppendChar('o');
	ss2.print();
	ss2.RemoveAt(20);
	ss2.print();
	SecureStringUnprotected ssu = ss2.GetUnprotected();
	SecureStringUnprotected ssu2(std::move(ssu));
	std::cout << ssu2.c_str();
	return 0;
}