#include "SecureStringUnprotected.h"

SecureStringUnprotected::SecureStringUnprotected(char* str)
{
	m_string = str;
}

SecureStringUnprotected::SecureStringUnprotected(SecureStringUnprotected && other)
{
	*this = std::move(other);
}

SecureStringUnprotected & SecureStringUnprotected::operator=(SecureStringUnprotected && rhs)
{
	if (this != &rhs)
	{
		free(this->m_string);
		this->m_string = rhs.m_string;

		rhs.m_string = nullptr;
	}
	return *this;
}

char * SecureStringUnprotected::c_str()
{
	return m_string;
}
