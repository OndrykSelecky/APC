#include "Sudoku.h"
#include <iostream>
#include <sstream>
#include <string>
#include <cmath>
#include <iomanip>

Sudoku::Sudoku()
	: grid(std::vector<std::vector<uint32_t>>(grid_size, std::vector<uint32_t>(grid_size)))
	
{		
	square_size = (uint32_t)std::sqrt(grid_size);
	create_blank_grid();
}

Sudoku::Sudoku(const std::string & serialized)	
	: grid(std::vector<std::vector<uint32_t>>(grid_size, std::vector<uint32_t>(grid_size)))
	
{
	square_size = (uint32_t)std::sqrt(grid_size);
	std::stringstream serialized_stream(serialized);
	
	for (auto row = 1; row <= grid_size; row++)
	{
		for (auto col = 1; col <= grid_size; col++)
		{			
			//read one number
			uint32_t number;			
			serialized_stream >> number;			
			if (serialized_stream.fail()) {
				std::cout << "Error! Serialization was not succesful\n";
				create_blank_grid();
				return;
			}
			else
			{
				set_number(row, col, number);
			}

		}
	}	
}

bool Sudoku::is_valid() const
{
	//for each number
	for (uint32_t number = 1; number <= grid_size; number++) {

		//check all rows for dupliacates
		for (uint32_t row = 1; row <= grid_size; row++) 
			if ((count_in_row(row, number) >1 )) return false;			
		
		//check all columns
		for (uint32_t col = 1; col <= grid_size; col++) 
			if ((count_in_column(col, number) > 1)) return false;			
		
		//check all squares
		for (uint32_t row = 1; row <= square_size; row++)
			for (uint32_t col = 1; col <= square_size; col++)
				if (count_in_square(row, col, number) > 1) return false;
				
	}

	return true;
}

bool Sudoku::solve()
{
	if (!is_valid()) return false;
	if (solve_grid()) return true;
	else return false;
}

uint32_t Sudoku::operator()(size_t i, size_t j) const
{
	return get_number(i,j);
}

uint32_t Sudoku::get_number(size_t i, size_t j) const
{
	if (out_of_grid_range(i) || out_of_grid_range(j)) return 0;
	else return grid[i-1][j-1];
}

bool Sudoku::set_number(size_t i, size_t j, uint32_t number)
{
	if (out_of_grid_range(i) || out_of_grid_range(j) || (number > grid_size)) return false;
	grid[i-1][j-1] = number;
	return true;
}

Sudoku & Sudoku::operator++(){	

	uint32_t row = 0;
	uint32_t col = 0;
	bool stop = false;

	//find blank position
	for (auto i = 1; i <= grid_size, !stop; i++)
		for (auto j = 1; j <= grid_size; j++)
			if (get_number(i, j) == 0)
			{
				row = i;
				col = j;
				stop = true;
				break;
			}	
	
	//make copy of grid
	std::vector<std::vector<uint32_t>> new_grid(grid);	

	//solve sudoku and copy number on position from solution to old grid
	if (solve()) {		
		uint32_t number = get_number(row, col);		
		grid = new_grid;		
		set_number(row, col, number);
	}	
	return *this;
}

std::string Sudoku::serialize() const
{
	std::string serialized_grid;
	for (auto row = 1; row <= grid_size; row++)
	{
		for (auto col = 1; col <= grid_size; col++)
		{
			serialized_grid += std::to_string(get_number(row, col))+" ";
		}
		serialized_grid += "\n";
	}

	return serialized_grid;
}

bool Sudoku::out_of_grid_range(size_t number) const
{
	if (number >= 1 && number <= grid_size) return false;	
	else return true;
}

void Sudoku::create_blank_grid()
{
	for (auto row = 1; row <= grid_size; row++)
		for (auto col = 1; col <= grid_size; col++) 
			set_number(row, col, 0);
}

bool Sudoku::find_free(uint32_t & row, uint32_t & col)
{
	for (row = 1; row <= grid_size; row++)
		for (col = 1; col <= grid_size; col++) 
			if (get_number(row, col) == 0) return true;
		
	return false;
}

uint32_t Sudoku::count_in_row(uint32_t row, uint32_t number) const
{
	uint32_t count = 0;
	for (auto col = 1; col <= grid_size; col++)	
		if (number == get_number(row, col))	count++;		
	
	return count;;
}

uint32_t Sudoku::count_in_column(uint32_t col, uint32_t number) const
{
	uint32_t count = 0;
	for (auto row = 1; row <= grid_size; row++)	
		if (number == get_number(row, col)) count++;
		
	return count;
}

uint32_t Sudoku::count_in_square(uint32_t square_row, uint32_t square_col, uint32_t number) const
{
	uint32_t count = 0;
	for (auto row = square_row*square_size; row > (square_row - 1)*square_size; row--)
		for (auto col = square_col*square_size; col > (square_col - 1)*square_size; col--)
			if (number == get_number(row, col)) count++;		
	
	return count;
}


bool Sudoku::solve_grid()
{
	uint32_t row, col;

	//find first blank position
	if (!find_free(row, col)) return true;	

	//try if all numbers are suitable for position
	for (auto number = 1; number <= grid_size; number++) 
	{
		if (!count_in_row(row, number) && !count_in_column(col, number)
			&& !count_in_square((row - 1) / square_size + 1, (col - 1) / square_size + 1, number))
		{
			//set number on position
			set_number(row, col, number);

			//try solve grid with position assigned
			if (solve_grid()) return true;	

			//if was not solved, undo changes
			set_number(row, col, 0);			
		}
	}
	
	//backtrack to previous assignment
	return false;
}


void print_border_line(std::ostream &lhs, const Sudoku & rhs, std::string dash)
{
	
	lhs << "+";
	for (uint32_t j = 1; j <= rhs.grid_size; j++)
	{
		lhs << dash;
		if (j % rhs.square_size != 0)
		{
			lhs << "-";
		}
		else
		{
			lhs << "+";
		}
	}
	lhs << std::endl;

}

std::ostream & operator<<(std::ostream & lhs, const Sudoku & rhs)
{
	std::string temp_str = std::to_string(rhs.grid_size);
	uint32_t number_length = temp_str.size();
	std::string dash;
	for (uint32_t i = 0; i < number_length; i++) dash.append("-");	
	
	print_border_line(lhs, rhs, dash);	

	for (uint32_t i = 1; i <= rhs.grid_size; i++)
	{				
		lhs << "|";		
		for (uint32_t j = 1; j <= rhs.grid_size; j++)
		{
			lhs << std::setw(number_length) << rhs.get_number(i, j);
			if (j % rhs.square_size != 0)
			{
				lhs << " ";
			}
			else
			{
				lhs << "|";
			}
		}
		lhs << std::endl;
		
		if (i % rhs.square_size == 0) 
		{
			print_border_line(lhs, rhs, dash);
		}
	}
	
	return lhs;
}
