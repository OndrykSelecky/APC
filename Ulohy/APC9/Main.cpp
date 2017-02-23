#include <iostream>
#include "Rainbow.h"
#include "sha1.hpp"
#include <thread>
#include <atomic>
#include <conio.h>

std::atomic_bool end;

//waits for pressing the "q" key
void wait(Rainbow& r)
{
	while (1)
	{		
		if (end == true) break;
		if (_kbhit() && getch() == 'q')
		{			
			std::lock_guard<std::mutex> lock(r.create_table.mutex);
			r.stop = true;
			break;
		}
	}
	
	std::cout << "Application is terminating...\n";
}


int main(int argc, char** argv)
{

	if (argc <= 3)
	{
		std::cerr << "Usage: --create <N> <path_to_file> for create mode, --search <hash> <path_to_file> for search mode\n";
		return 1;
	}

	std::string mode = argv[1];
	if (mode == "--create")
	{
		int N;
		try
		{
			N = std::stoi(argv[2]);
		}
		catch (std::invalid_argument e)
		{
			std::cerr << "Error: argument <N> is invalid 2\n";
			return 1;
		}

		Rainbow r;
		end = false;

		std::thread wait_thread(wait, std::ref(r));

		r.create(argv[3], N);

		end = true;

		wait_thread.join();
	}
	else if (mode == "--search")
	{
		Rainbow r;
		
		r.search(argv[2], argv[3]);
	}
	else
	{
		std::cerr << "Error: incorrect mode name\n";
		return 1;
	}

	return 0;
}