#pragma once
#include <iostream>
#include <fstream>
#include <vector>

#include "sha1.hpp"
#include "CreateTable.h"
#include "SearchTablePart.h"

class Rainbow
{
public:
	Rainbow() : stop(false) {};

	void Rainbow::create(const std::string& file_name, int max_password_length);
	void Rainbow::search(const std::string& hash, const std::string& file_name);

	//table in create mode
	CreateTable create_table;

	//table in search mode, each element is table for password of same length
	std::vector<SearchTablePart> search_table;

	//used for stopping the execution
	bool stop;
	
private:	
	//generates starting keys
	void generate();

	//compute hash
	std::string hash(const std::string& key, SHA1& sha1);

	//reduce function, 
	std::string reduce(const std::string& hash, uint64_t offset, int pass_length);

	friend void compute_chains(Rainbow& rainbow, uint64_t lower, uint64_t upper, int index);

};

//compute chains in create mode
void compute_chains(Rainbow& rainbow, uint64_t lower, uint64_t upper, int index);