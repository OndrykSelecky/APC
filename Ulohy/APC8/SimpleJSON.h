#pragma once
#include <iostream>
#include <vector>
#include "Record.h"

/*
	Ondrej Selecký
*/

class SimpleJSON
{
public:

	SimpleJSON() {};

	//constructor, reads from ofstream
	SimpleJSON(std::ifstream& input_stream);


	SimpleJSON(SimpleJSON& other);

	SimpleJSON(SimpleJSON&& other);

	SimpleJSON &operator=(SimpleJSON& rhs);

	SimpleJSON &operator=(SimpleJSON&& rhs);

	//if key is string, returns true. If key is number or JSON doesn't contain key, return false
	bool is_string(const std::string& key);

	//if key is number, returns true. If key is string or JSON doesn't contain key, return false
	bool is_num(const std::string& key);

	//returns true, if JSON contains key
	bool contains(const std::string& key);
	

	std::string get_value(std::string& key);

	std::string get_string_value(std::string & key);

	int64_t get_int_value(std::string& key);

	//write ti output stream
	void write(std::ofstream& output_stream);

	//print to ostream (name: value)
	void print(std::ostream& output_stream) const;

	//add pair with string value, if JSON already contains key, it is not changed and false is returned
	bool add(std::string key, std::string value);		

	//same as add(key, string value), but with number
	bool add(std::string key, int64_t value);

	//remove pair with key, returns false if JSON doesn't contains key
	bool remove(std::string key);

private:

	std::vector<Record> m_content;

	const unsigned int JSON_MAX_STRING_SIZE = 2048;

	//finds position of pair with key "key"
	std::vector<Record>::iterator find_position(const std::string& key);

	//read from input stream
	void read_json(std::ifstream& input_stream);

	//read whitespace until deim is found. If there is non-white space before delim or delim is not found, throw exception 
	void read_white(std::ifstream& input_stream, char delim);

	/*
	read whitespace until delim or end is found, return 1 if delim is found or 2 if end is found first. 
	Throws esception if nothing or other than whitespace, delim or end is found
	*/
	int read_white_to_end(std::ifstream & input_stream, char delim, char end);	

};
