#include "SecureString.h"
#include "SecureStringUnprotected.h"

#include <iostream>

SecureString::SecureString()
{

	init_empty();

};

SecureString::SecureString(const char * str)
{
	//adds '\0' at the end of string
	m_length = strlen(str)+1;
	m_buffer_length = count_buffer_length(m_length);	
		

	m_string = (char *) malloc(m_buffer_length*sizeof(char));
	

	//encryption dont work for const char and we must copy input string
	memcpy(m_string, str, m_length-1);
	
	for (size_t i = m_length-1; i < m_buffer_length; i++) m_string[i] = '\0';


	Protect(m_string, m_buffer_length);	
}

SecureString::SecureString(SecureString & other)
{
	*this = other;
}

SecureString::SecureString(SecureString && other)
{	
	*this = std::move(other);
}

SecureString::~SecureString()
{
	CleanMemory(m_string, m_buffer_length);
}

SecureString & SecureString::operator=(SecureString & rhs)
{
	if (this != &rhs)
	{
		memcpy(this->m_string, rhs.m_string, rhs.m_length);
		this->m_length = rhs.m_length;
		this->m_buffer_length = rhs.m_buffer_length;
	}
	return *this;
}

SecureString & SecureString::operator=(SecureString && rhs)
{
	if (this != &rhs)
	{
		free(this->m_string);
		this->m_string = rhs.m_string;
		this->m_length = rhs.m_length;
		this->m_buffer_length = rhs.m_buffer_length;

		rhs.m_buffer_length = 0;
		rhs.m_length = 0;
		rhs.m_string = nullptr;
	}
	return *this;
}

SecureString & SecureString::operator+=(SecureString & rhs)
{

	if (this->m_buffer_length == 0) *this = rhs;
	if (rhs.m_buffer_length == 0) return *this;

	if (this != &rhs)
	{
		//count new lengths
		size_t new_length = m_length + rhs.m_length - 1;
		size_t new_buffer_length = count_buffer_length(new_length);

		Unprotect(m_string, m_buffer_length);
		Unprotect(rhs.m_string, rhs.m_buffer_length);

		//resize buffer if needed
		if (new_buffer_length > m_buffer_length)
		{
			m_string = (char*)realloc(m_string, new_buffer_length);
			if (m_string == nullptr)
			{
				std::cerr << "Chyba pri realokacii\n";
				std::terminate();
			}
		}

		memcpy(m_string + m_length-1, rhs.m_string, rhs.m_length);
		for (size_t i = new_length; i < new_buffer_length; i++) m_string[i] = '\0';

		
		Protect(m_string, new_buffer_length);
		Protect(rhs.m_string, rhs.m_buffer_length);
		

		m_length = new_length;
		m_buffer_length = new_buffer_length;
		
	}
	else
	{
		
		size_t new_length = (m_length << 1) - 1;
		size_t new_buffer_length = count_buffer_length(new_length);

		Unprotect(m_string, m_buffer_length);

		if (new_buffer_length > m_buffer_length)
		{
			m_string = (char*)realloc(m_string, new_buffer_length);
			if (m_string == nullptr)
			{
				std::cerr << "Chyba pri realokacii\n";
				std::terminate();
			}
		}

		memcpy(m_string + m_length - 1, m_string, m_length);
		for (size_t i = new_length; i < new_buffer_length; i++) m_string[i] = '\0';
				
		Protect(m_string, new_buffer_length);
				
		m_length = new_length;
		m_buffer_length = new_buffer_length;

	}
	
	return *this;
}

size_t SecureString::Length()
{
	return m_length-1;
}

void SecureString::Clear()
{
	CleanMemory(m_string, m_buffer_length);
	init_empty();
}

void SecureString::SetAt(size_t position, char new_char)
{

	if (position >= m_length) return;
	if (position == m_length - 1) return AppendChar(new_char);

	Unprotect(m_string, m_buffer_length);
	m_string[position] = new_char;
	Protect(m_string, m_buffer_length);
}

void SecureString::AppendChar(char new_char)
{
	size_t new_length = m_length +1;
	size_t new_buffer_length = count_buffer_length(new_length);

	Unprotect(m_string, m_buffer_length);

	if (new_buffer_length > m_buffer_length)
	{
		m_string = (char*)realloc(m_string, new_buffer_length);
		if (m_string == nullptr)
		{
			std::cerr << "Chyba pri realokacii\n";
			std::terminate();
		}
	}

	m_string[m_length-1] = new_char;
	for (size_t i = new_length-1; i < new_buffer_length; i++) m_string[i] = '\0';

	Protect(m_string, new_buffer_length);	

	m_length = new_length;
	m_buffer_length = new_buffer_length;
}

void SecureString::RemoveAt(size_t position)
{

	if (position >= m_length) return;

	size_t new_length = m_length - 1;
	size_t new_buffer_length = count_buffer_length(new_length);

	Unprotect(m_string, m_buffer_length);

	
	for (size_t i = position; i < new_length; i++) m_string[i] = m_string[i + 1];

	if (new_buffer_length > m_buffer_length)
	{
		m_string = (char*)realloc(m_string, new_buffer_length);
		if (m_string == nullptr)
		{
			std::cerr << "Chyba pri realokacii\n";
			std::terminate();
		}
	}

	
	Protect(m_string, new_buffer_length);
	

	m_length = new_length;
	m_buffer_length = new_buffer_length;



}

SecureStringUnprotected SecureString::GetUnprotected() const
{
	char *unprotected = (char*)malloc(m_length * sizeof(char));

	Unprotect(m_string, m_buffer_length);

	//string in *this should stay protected, so we copy it to new one
	memcpy(unprotected, m_string, m_length);
	Protect(m_string, m_buffer_length);	

	return SecureStringUnprotected(unprotected);
	//std::cout << ssu.c_str();

}

void SecureString::print()
{
	std::cout << "buffer_length: " << m_buffer_length << ", length: " << m_length << std::endl;
	Unprotect(m_string, m_buffer_length);
	std::cout << m_string << std::endl;
	Protect(m_string, m_buffer_length);
}

size_t SecureString::count_buffer_length(size_t buffer_length)
{
	if (buffer_length == 0) return SECURE_MEMORY_BLOCK;

	uint8_t difference = buffer_length % SECURE_MEMORY_BLOCK;
	if (difference) buffer_length += SECURE_MEMORY_BLOCK - difference;
	return buffer_length;
}

void SecureString::init_empty()
{
	m_buffer_length = count_buffer_length(0);
	m_length = 1;
	m_string = (char *)malloc(m_buffer_length * sizeof(char));
	for (int i = 0; i < m_buffer_length; i++) m_string[i] = '\0';

	Protect(m_string, m_buffer_length);
}
