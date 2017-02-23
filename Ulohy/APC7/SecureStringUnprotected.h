#pragma once
#include "Protect.h"
#include <iostream>

class SecureStringUnprotected {
public:
	SecureStringUnprotected() {};
	SecureStringUnprotected(char *str);
	
	SecureStringUnprotected(SecureStringUnprotected&& other);
	SecureStringUnprotected &operator=(SecureStringUnprotected&& rhs);

	char* c_str();

private:
	char* m_string = nullptr;
};
