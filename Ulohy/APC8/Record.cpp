#include "Record.h"
#include <string>

Record::Record(Record & other)
{
	*this = other;
}

Record::Record(Record && other)
{
	*this = std::move(other);
}

Record & Record::operator=(Record & rhs)
{
	if (this != &rhs)
	{
		this->m_is_string = rhs.m_is_string;
		this->m_name = rhs.m_name;
		this->m_value = rhs.m_value;
	}
	return *this;
}

Record & Record::operator=(Record && rhs)
{		
	if (this != &rhs)
	{
		this->m_is_string = rhs.m_is_string;
		this->m_name = std::move(rhs.m_name);
		this->m_value = std::move(rhs.m_value);
	}
	return *this;
}
