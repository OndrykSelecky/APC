#pragma once
#include <iostream>

struct Record
{
	Record() {};

	Record(std::string& name, std::string& value, bool is_string)
		: m_is_string(is_string)
		, m_name(name)
		, m_value(value) {};
	
	Record(Record& other);

	Record(Record&& other);


	Record &operator=(Record& rhs);

	Record &operator=(Record&& rhs);


	//name (key in JSON)
	std::string m_name;

	//value (as string)
	std::string m_value;

	//tru = string, false = number
	bool m_is_string;

};