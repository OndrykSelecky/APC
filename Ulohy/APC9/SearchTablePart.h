#pragma once
#include <vector>
#include <string>

//structure used for search mode.
class SearchTablePart
{
public:
	std::vector<std::pair<std::string, std::string>> m_lines;
	int m_password_length;
	uint64_t m_chain_length;
	uint64_t m_chain_num;
};
