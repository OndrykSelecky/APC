#pragma once
#include <iostream>
#include <map>
#include <mutex>
#include <vector>

//structure used for create mode

class CreateTable
{
public:
	//length of actual chain
	uint64_t m_chain_length;
	uint64_t m_chain_num;
	int m_akt_password_length;

	//contains generated start keys foreach chain
	std::vector<std::string> m_start_keys;

	//rainbow table
	std::multimap<std::string, std::string> m_table;

	//mutex used for writing to m_table
	std::mutex mutex;
};
