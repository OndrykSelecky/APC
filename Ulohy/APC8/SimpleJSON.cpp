#include "SimpleJSON.h"
#include <string>
#include <fstream>
#include <sstream>

SimpleJSON::SimpleJSON(std::ifstream& input_stream)
{
	try {
		read_json(input_stream);
	}
	catch (const std::invalid_argument& e) {
		m_content.clear();
		throw e;
	}
	
}

SimpleJSON::SimpleJSON(SimpleJSON & other)
{
	*this = other;
}

SimpleJSON::SimpleJSON(SimpleJSON && other)
{
	*this = std::move(other);
}

SimpleJSON & SimpleJSON::operator=(SimpleJSON & rhs)
{
	if (this != &rhs)
	{
		this->m_content = rhs.m_content;
	}
	return *this;
}

SimpleJSON & SimpleJSON::operator=(SimpleJSON && rhs)
{
	if (this != &rhs)
	{
		this->m_content.clear();
		this->m_content = std::move(rhs.m_content);
	}
	return *this;
}

bool SimpleJSON::is_string(const std::string& key)
{
	auto it = find_position(key);
	if (it != m_content.end())
	{
		return it->m_is_string;
	}

	return false;
}

bool SimpleJSON::is_num(const std::string & key)
{
	auto it = find_position(key);
	if (it != m_content.end())
	{
		return !it->m_is_string;
	}

	return false;
}

bool SimpleJSON::contains(const std::string & key)
{
	auto it = find_position(key);
	if (it != m_content.end())
	{
		return true;
	}
	return false;
}


std::string SimpleJSON::get_value(std::string & key)
{
	auto it = find_position(key);
	if (it != m_content.end())
	{
		return it->m_value;
	}

	throw std::invalid_argument("Invalid input: there is no record for key in JSON\n");
}

std::string SimpleJSON::get_string_value(std::string & key)
{
	auto it = find_position(key);
	if (it != m_content.end() && it->m_is_string)
	{
		return it->m_value;
	}

	throw std::invalid_argument("Invalid input: there is no record for key in JSON\n");
}

int64_t SimpleJSON::get_int_value(std::string & key)
{
	auto it = find_position(key);
	if (it != m_content.end() && !it->m_is_string)
	{
		std::stringstream ss(it->m_value);
		int64_t number = 0;
		if (!(ss >> number)) throw std::invalid_argument("Error: can't convert from string to int64_t\n");
		return number;
	}

	throw std::invalid_argument("Invalid input: there is no record for key in JSON\n");
}

void SimpleJSON::write(std::ofstream & output_stream)
{
	output_stream << "{\n";
	for (std::vector<Record>::iterator it = m_content.begin(); it != m_content.end(); it++)
	{
		output_stream << "\"" << it->m_name << "\": ";
		if (it->m_is_string) output_stream << "\"";
		output_stream << it->m_value;
		if (it->m_is_string) output_stream << "\"";
		if (it + 1 != m_content.end()) output_stream << ",";
		output_stream << "\n";
	}
	output_stream << "}";
}

void SimpleJSON::print(std::ostream & output_stream) const
{
	for (const Record& r : m_content)
	{
		output_stream << r.m_name << ": ";
		output_stream << r.m_value;
		output_stream << "\n";
	}
}

bool SimpleJSON::add(std::string key, std::string value)
{
	if (is_string(key)) return false;
	m_content.push_back(Record(key, value, true));	
	return true;
}


bool SimpleJSON::add(std::string key, int64_t value)
{
	if (is_string(key)) return false;
	m_content.push_back(Record(key, std::to_string(value), false));
	return true;
}

bool SimpleJSON::remove(std::string key)
{
	auto it = find_position(key);
	if (it != m_content.end())
	{
		m_content.erase(find_position(key));
		return true;
	}
	return false;
}


std::vector<Record>::iterator SimpleJSON::find_position(const std::string & key)
{
	
	for (auto it = m_content.begin(); it != m_content.end(); it++)
	{
		if (it->m_name == key) return it;
	}
	return m_content.end();
}

void SimpleJSON::read_json(std::ifstream & input_stream)
{

	//check, if there is {
	read_white(input_stream, '{');
	
	//determine, if json is not empty (first non-white character is ")
	int end = read_white_to_end(input_stream, '"', '}');	
	if (end == 2) return;

	char *buffer = new char[JSON_MAX_STRING_SIZE];

	while (!input_stream.eof())
	{
		//read key
		input_stream.getline(buffer, JSON_MAX_STRING_SIZE, '"');		
		std::string key(buffer);

		//first non-white is :
		read_white(input_stream, ':');

		//decide, if next part is number or string
		while (!input_stream.eof())
		{
			char c = input_stream.get();
			if (c == '"')
			{
				input_stream.getline(buffer, JSON_MAX_STRING_SIZE, '"');
				
				m_content.push_back(Record(key, std::string(buffer), true));
				break;
			}
			else if (isdigit(c) || c=='-')
			{
				input_stream.putback(c);
				int64_t number;
				if (!(input_stream >> number)) throw std::invalid_argument("Invalid input file\n");
				m_content.push_back(Record(key, std::to_string(number), false));
				break;
			}
			else if (!isspace(c)) throw std::invalid_argument("Invalid input file\n");
		}
		
		//if next non-white is }, it is end. If it is ',', we continue search
		int end = read_white_to_end(input_stream, ',', '}');
		if (end == 2) return;		


		//find first '"' of new key
		read_white(input_stream, '"');

	}
	
}

void SimpleJSON::read_white(std::ifstream& input_stream, char delim)
{
	while (!input_stream.eof())
	{
		char c = input_stream.get();
		
		if (c == delim) {
			return;
		}
		if (!isspace(c)) break;
	}
	throw std::invalid_argument("Invalid input file");

}

int SimpleJSON::read_white_to_end(std::ifstream& input_stream, char delim, char end)
{
	while (!input_stream.eof())
	{
		char c = input_stream.get();

		if (c == delim)
			return 1;

		else if (c == end)
			return 2;
		else if (!isspace(c))
			break;
	}
	throw std::invalid_argument("Invalid input file");
}
