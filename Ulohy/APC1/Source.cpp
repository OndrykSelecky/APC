//Ondrej Seleckı, APC uloha 1
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <utility>
#include <cstdint>
#include <string>
#include <algorithm>

using namespace std;

//checks if line has valid format
int check(string input_string, int &line_number) {
	bool number_found = false;			//if number was found
	bool end_of_number = false;			//only 1 number is valid
	bool error = false;					//other character

	for (char line_char : input_string) {
		if (isdigit(line_char)) {
			if (end_of_number) {
				error = true;
				break;
			}
			if (!number_found) {
				number_found = true;
			}			
		}
		else if (isspace(line_char)) {
			if (number_found && !end_of_number) {
				end_of_number = true;
			}
		}
		else {
			error = true;
			break;
		}
	}

	if (!number_found || error) {
		cout << "Line " << line_number << ": Invalid number format." << endl;
		return 1;
	}

	return 0;
}



int main(int argc, char **argv) {	
	if (argc != 3) {
		cout << "Wrong number of parameters!" << endl;
		return 1;
	}

	ifstream input_file(argv[1]);
	if (!input_file.is_open()) {
		cout << "Error with input file" << endl;
		return 1;
	}

	vector<pair<uint32_t, string>> lines;
	string input_string;
	int line_number = 0;
	while (getline(input_file, input_string)) {
		if (input_file.fail()) {
			cout << "Error reading input file" << endl;
			return 1;
		}

		line_number++;
		if (check(input_string, line_number)) {
			return 1;
		}

		//gets number from line
		stringstream input_stream(input_string);
		uint32_t input_number = 0;
		input_stream >> input_number;
		if (input_stream.fail()) {
			cout << "Line " << line_number << ": Number is out of range." << endl;
			return 1;
		}		
		lines.push_back(pair<uint32_t, string> (input_number, input_string));
	}
	
	sort(lines.begin(), lines.end());

	ofstream output_file(argv[2]);
	if (!output_file.is_open()) {
		cout << "Error with output file" << endl;
		return 1;
	}

	for (pair<uint32_t, string> line : lines) {
		output_file << line.second << endl;
	}
	
	return 0;
}