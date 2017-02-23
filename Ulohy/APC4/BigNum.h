#pragma once
#include <vector>

class BigNum
{
public:
	BigNum();
	BigNum(int64_t n);
	explicit BigNum(const std::string& str_number);
	
	std::string to_string();

	BigNum(const BigNum& other);
	BigNum& operator=(const BigNum& rhs);
		
	bool operator==(const BigNum& rhs) const;
	bool operator!=(const BigNum& rhs) const;
	bool operator<(const BigNum& rhs) const;
	bool operator>(const BigNum& rhs) const;
	bool operator<=(const BigNum& rhs) const;
	bool operator>=(const BigNum& rhs) const;

	BigNum BigNum::operator-() const;
	const BigNum& BigNum::operator+() const;

	BigNum BigNum::operator+(const BigNum& rhs) const;
	BigNum BigNum::operator-(const BigNum& rhs) const;
	BigNum BigNum::operator*(const BigNum& rhs) const;

	BigNum& BigNum::operator+=(const BigNum& rhs);
	BigNum& BigNum::operator-=(const BigNum& rhs);
	BigNum& BigNum::operator*=(const BigNum& rhs);

private:		
	//private constructor, creates BigNum from vector of numbers and sign int
	BigNum(std::vector<uint8_t> number, int sign);

	//each digit is represented as uint8_t number in vector
	std::vector<uint8_t> m_number;

	//1 == +, -1 == - 
	int m_sign;

	friend BigNum abs(const BigNum& num);
};

BigNum abs(const BigNum& num);