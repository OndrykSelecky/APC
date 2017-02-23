/*
autor: Ondrej Seleckı

Zo súboru sa naèítava do buffera. V kadom bufferi (okrem posledného) sa nájde len ten vzor, ktorı sa v aktuálnom bufferi nachádza celı
aj s prefixom a suffixom (aby ho bolo moné hneï vypísa, zaèiatok a koniec súboru sú ošetrené). Do nového buffera sa na
zaèiatok skopíruje koniec zo starého s dåkou prefix + suffix + dåka_vzoru - 1, aby sa dali nájs aj vıskyty, ktoré boli
rozdelené dvomi buffermi
*/

#include <iostream>
#include <fstream>
#include <random>
#include <string>

#define buffer_length 4096
#define prefix_size 3
#define max_pattern_length 128

//generates text file
void generate(uint64_t n)
{
	std::ofstream output_stream;
	output_stream.open("textfile.txt");

	std::random_device r;
	std::default_random_engine e1(r());

	for (auto i = 0; i < n; i++)
	{
		std::uniform_int_distribution<int> uniform_dist('a', 'c'+2);

		int num = uniform_dist(e1);
		char output_char;
		if (num == 'c' + 1) output_char = '\n';
		else if (num == 'c' + 2) output_char = '\t';
		else output_char = num;
		output_stream << output_char;
	}
	output_stream.close();

}


//reads string from stream of size size
std::string read_string(std::ifstream& input_stream, size_t size)
{
	std::string input_string(size, '\0');
	input_stream.read(&input_string[0], size);
		
	return input_string;
}

//writes string with \n and \t
std::string write_string(std::string input_string)
{
	if (input_string.empty()) return input_string;
	std::string output_string;
	for (auto character : input_string)
	{
		if (character == '\n') output_string.append("\\n");
		else if (character == '\t') output_string.append("\\t");
		else output_string.append(1,character);
	}
	return output_string;
}

int main(int argc, char **argv)
{
	if (argc != 3)
	{
		std::cerr << "Error: Wrong number of parameters" << std::endl
			<< "Parameters: text_file pattern" << std::endl;
		return -1;
	}
	//generate(1000);	

	//searching pattern
	std::string pattern(argv[2]);

	if (pattern.length() > 128)
	{
		std::cerr << "Error: Pattern is too long" << std::endl;
		return -1;
	}

	if (pattern.length() > buffer_length)
	{
		std::cerr << "Error: Buffer length is too small" << std::endl;
		return -1;
	}

	std::ifstream input_stream;
	input_stream.open(argv[1]);

	if (!input_stream.is_open())
	{
		std::cerr << "Error: File can't be opened" << std::endl;
		return -1;
	}
	
	//length of offset, that is copied from previous buffer, length of prefix + length of pattern + length of suffix
	size_t offset_length = 2 * prefix_size + pattern.length() - 1;


	//position of first character in buffer
	int64_t actual_position = 1;

	//read first string
	std::string searched_string = read_string(input_stream, buffer_length);
	
	//if true, pattern can be searched to end of buffer. Used in last buffer
	bool to_end = false;

	while (1)
	{
		//std::cout << "Line (" << actual_position << "): " << write_string(searched_string) << std::endl;
		
		//file is read, add 3 \0 to the end of file
		if (!input_stream.gcount())
		{
			searched_string.append(prefix_size, '\0');			
			to_end = true;
		}

		//position at which to start the search
		size_t position_offset = (actual_position == 1) ? 0 : prefix_size;
		
		
		//will store position of the first character of the found pattern
		size_t searched_position;

		while ((searched_position = searched_string.find(pattern, position_offset)) != std::string::npos)
		{
			//check, if sufix of found pattern is in buffer, print result
			if (searched_position <= searched_string.length() - prefix_size - pattern.length() || to_end)
			{
				
				//position in string where prefix of found pattern starts
				size_t prefix_start_position;

				//length of prefix
				size_t prefix_length;

				if (prefix_size <= searched_position )
				{
					prefix_start_position = searched_position - prefix_size;
					prefix_length = prefix_size;
				}
				else
				{
					prefix_start_position = 0;
					prefix_length = searched_position;					
				}

				//write position, prefix and sufix
				std::string prefix = write_string(searched_string.substr(prefix_start_position, prefix_length));
				std::string suffix = write_string(searched_string.substr(searched_position + pattern.length(), prefix_size));
				size_t position = searched_position + actual_position;
				std::cout << position  << ": " << prefix << "..." << suffix << std::endl;
			}

			//shift position_offset by 1, so same pattern is not found again
			position_offset = searched_position + 1;
		}	

		if (to_end) break;

		//read new buffer
		std::string input_buffer = read_string(input_stream, buffer_length);		

		//count actual:position, first buffer is shorter because ther is no addition from previuos
		if (actual_position == 1) actual_position -= offset_length;
		actual_position += buffer_length;

		//create new search string: end of previous string + new buffer
		std::string new_string = searched_string.substr(searched_string.length() - offset_length, offset_length) + input_buffer;
		searched_string = new_string;
		
	}

	return 0;
}