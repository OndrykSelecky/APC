#include "Rainbow.h"


#include <fstream>
#include <random>
#include <cmath>
#include <thread>
#include <string>

void Rainbow::create(const std::string& file_name, int max_password_length)
{
	std::ofstream output_file;
	output_file.open(file_name);
	if (!output_file.is_open())
	{
		std::cerr << "Error: file could't be opened";
		return;
	}

	output_file << max_password_length << "\n";
	create_table.m_akt_password_length = max_password_length;

	//determine number of threads
	int thread_num = std::thread::hardware_concurrency();
	if (thread_num < 2) thread_num = 1;
	

	//compute for each password length from 2 to max
	while (create_table.m_akt_password_length > 1)
	{
		//compute parameters of chains
		int p_size = create_table.m_akt_password_length / 2;
		create_table.m_chain_num = static_cast<uint64_t>(std::pow(10, p_size)*2);
		create_table.m_chain_length = static_cast<uint64_t>(std::pow(10, create_table.m_akt_password_length - p_size));

		
		generate();


		std::thread *thread = new std::thread[thread_num-1];

		for (auto i = 0; i < thread_num-1; i++)
		{
			thread[i] = std::thread(compute_chains, std::ref(*this), i*create_table.m_chain_num / thread_num, (i + 1)*create_table.m_chain_num / thread_num, i);
		}

		compute_chains(std::ref(*this), (thread_num - 1)*create_table.m_chain_num / thread_num, (thread_num)*create_table.m_chain_num / thread_num, 15);

		for (auto i = 0; i < thread_num-1; i++)
		{
			thread[i].join();
		}
		
		
		//write output
		output_file << "l " << create_table.m_akt_password_length << "\n";
		for (const auto& pair : create_table.m_table)
		{
			output_file << pair.second << " " << pair.first << "\n";
		}
		create_table.m_table.clear();
		create_table.m_akt_password_length--;

		std::lock_guard<std::mutex> lock(create_table.mutex);
		if (stop) break;
	}		

	output_file.close();

}

void Rainbow::search(const std::string& hash, const std::string & file_name)
{
	std::ifstream input_file;
	input_file.open(file_name);
	if (!input_file.is_open())
	{
		std::cerr << "Error: file could't be opened";
		return;
	}

	int max_password_length;
	input_file >> max_password_length;
	
	
	
	//read table from file
	while (!input_file.eof())
	{
		std::string first, last;
		input_file >> first;
		input_file >> last;
		if (first == "l") 
		{
			SearchTablePart part;
			part.m_password_length = std::stoi(last);
			int p_size = part.m_password_length / 2;
			part.m_chain_num = static_cast<uint64_t>(std::pow(10, p_size)*2);
			part.m_chain_length = static_cast<uint64_t>(std::pow(10, part.m_password_length - p_size));
			search_table.push_back(part);
			continue;
		}
		search_table.back().m_lines.push_back(std::make_pair(first, last));
	}

	search_table.back().m_lines.pop_back();
	
	std::cout << hash << ": ";
	SHA1 sha1;

	//manually check passwords with length 1
	for (int i = 0; i < 10; i++)
	{
		if (this->hash(std::to_string(i), sha1) == hash)
		{
			std::cout << i << "\n";
			return;
		}
	}

	//find password for other password lengths
	for (std::vector<SearchTablePart>::reverse_iterator it = search_table.rbegin(); it < search_table.rend(); it++)
	{				
		for (int64_t last_offset = it->m_chain_length - 1; last_offset >= 0; last_offset--)
		{
			std::string act_hash = hash;
			std::string key;
			
			for (uint64_t offset = last_offset; offset < it->m_chain_length; offset++)
			{
				key = reduce(act_hash, offset, it->m_password_length);
				act_hash = this->hash(key, sha1);				
			}
			
			for (const auto& pair : it->m_lines)
			{
				if (pair.second == key)
				{
					
					key = pair.first;
					for (auto i = 0; i <= last_offset; i++)
					{
						act_hash = this->hash(key, sha1);
						if (hash == act_hash)
						{
							std::cout << key << "\n";
							return;
						}
						key = reduce(act_hash, i, it->m_password_length);
					}
					
				}
			}
			
		}
	}

	std::cout << "not found\n";

}

void Rainbow::generate()
{
	
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> dis(0, 9);

	create_table.m_start_keys.clear();

	int i=0;
	while ( i < create_table.m_chain_num)
	{
		
		std::string s;// = std::to_string(dis(gen));
		for (int j = 0; j < create_table.m_akt_password_length; j++)
		{
			s.append(std::to_string(dis(gen)));
		}
		if (std::find(create_table.m_start_keys.begin(), create_table.m_start_keys.end(), s) == create_table.m_start_keys.end())
		{
			create_table.m_start_keys.push_back(s);
			i++;
		}
	}
	
}

std::string Rainbow::hash(const std::string& key, SHA1& sha1)
{	
	sha1.update(key);
	return sha1.final();
}

//reduce function - from hash get first pass_length numbers, starting at position offset % hash_length
std::string Rainbow::reduce(const std::string& hash, uint64_t offset, int pass_length)
{
	std::string new_key = "";
	
	int char_count = 0;
	int start_position = static_cast<int>(offset % hash.length());
	int position = start_position;
	size_t hash_length = hash.length();

	while (char_count < pass_length)
	{
		char c = hash.at(position);
		if (c >= '0' && c <= '9')
		{
			new_key += c;
			char_count++;
			if (char_count == pass_length)
			{
				break;
			}
		}

		position = (position + 1) % hash_length;

		if (char_count == 0 && position == start_position)
		{
			for (char_count; char_count < pass_length; char_count++) new_key += '0';
		}
	}
		

	return new_key;
}

void compute_chains(Rainbow& rainbow, uint64_t lower, uint64_t upper, int index)
{
	
	SHA1 sha1;

	for (uint64_t i = lower; i < upper; i++)
	{				
		std::string key = rainbow.create_table.m_start_keys[i];
		for (auto offset = 0; offset < rainbow.create_table.m_chain_length; offset++)
		{			
			std::string hash = rainbow.hash(key, sha1);
			key = rainbow.reduce(hash, offset, rainbow.create_table.m_akt_password_length);
		}
		
		std::lock_guard<std::mutex> lock(rainbow.create_table.mutex);		
		rainbow.create_table.m_table.insert(std::pair<std::string, std::string>(key, rainbow.create_table.m_start_keys[i]));
		if (rainbow.stop) return;
	}
	
}
