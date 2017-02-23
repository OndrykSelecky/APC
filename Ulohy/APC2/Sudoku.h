#pragma once

#include <string>
#include <iostream>
#include <vector>


class Sudoku
{
public:
    /**
    * Creates empty sudoku. 
    */
    Sudoku();

	
    /**
    * Creates sudoku from some text serialized state (created by 
    * call to serialize). It should detect corruption of serialized 
    * data (unexpected format) in that case empty sudoku is created.  
    */
    explicit Sudoku(const std::string& serialized);

    /**
    * Checks if current sudoku state doesn't violates the rules. 
    */
    bool is_valid() const;

    /**
    * Fill all blanks, returns true if the solution exists, 
    * otherwise false. 
    */
    bool solve();

    /**
    * The same as GetNumber(i, j).
    */
	uint32_t operator()(size_t i, size_t j) const;
	

    /**
    * Gets number on position i, j. Position is 1 based from
    * top left corner. Should return 0 if i or j is larger than
    * size of sudoku. Also it should return 0 if 
    * the square is blank. 
    */
    uint32_t get_number(size_t i, size_t j) const;

    /**
    * Sets number on position i, j. Position is 1 based from
    * top left corner. Should return false if i or j is larger than
    * size of sudoku. Or the number is out of range. 
    */
    bool set_number(size_t i, size_t j, uint32_t number); 

    /**
    * Fill one empty square with correct solution. If sudoku is 
    * complete or no solution is possible ++ will have no effect. 
    */
    Sudoku& operator++();

    /**
    * Serialize sudoku do string is may not be pretty. Serialized 
    * state should work with appropriate constructor. 
    */
    std::string serialize() const;

	uint32_t get_grid_size() const { return grid_size; }

private:
	/**
	* Size of grid (rows and columns)
	*/
	static const uint32_t grid_size { 9 };

	/**
	* Size of square in sudoku
	*/
	uint32_t square_size; 

	/**
	* Grid with numbers
	*/
	std::vector<std::vector<uint32_t>> grid;	

	/**
	* Return true if number is not valid number of row or column
	*/
	bool out_of_grid_range(size_t number) const;

	/**
	* Creates sudoku grid with all zeros
	*/
	void create_blank_grid();

	/** 
	* Returns true if blank field in sudoku is found
	*/
	bool find_free(uint32_t& row, uint32_t& col);

	/**
	* count how many times is number in row row
	*/
	uint32_t count_in_row(uint32_t row, uint32_t number) const;

	/**
	* count how many times is number in column col
	*/
	uint32_t count_in_column(uint32_t col, uint32_t number) const;

	/**
	* count how many times is number in square with coordinate square_row and square_col (from upper left corner)
	*/
	uint32_t count_in_square(uint32_t square_row, uint32_t square_col, uint32_t number) const;

	/**
	* Solves one grid using backtracking
	*/
	bool solve_grid();

	friend std::ostream& operator<<(std::ostream& lhs, const Sudoku& rhs);
	friend void print_border_line(std::ostream &lhs, const Sudoku & rhs, std::string dash);
};

/*
* Prints line between squares
*/
void print_border_line(std::ostream &lhs, const Sudoku & rhs, std::string dash);

/**
* Prints Sudoku board to selected output stream. 
*/
std::ostream& operator<<(std::ostream& lhs, const Sudoku& rhs);


