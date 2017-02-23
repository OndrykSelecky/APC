#include "BigNum.h"
#include <cctype>
#include <iostream>
#include <string>
#include <vector>

BigNum::BigNum()
{
	m_sign = 1;
	m_number.push_back(0);	
}

BigNum::BigNum(int64_t n)
{
	if (n < 0)
	{
		m_sign = -1;
		n *= -1;
	}
	else
	{
		m_sign = 1;
	}

	while (n > 0)
	{
		m_number.push_back(n % 10);
		n /= 10;
	}

	std::reverse(m_number.begin(), m_number.end());

}

BigNum::BigNum(const std::string & str_number)
{

	std::string::const_iterator it = str_number.begin();

	//detect sign
	if (str_number[0] == '-')
	{
		m_sign = -1;
		it++;
	}
	else
	{
		if (str_number[0] == '+') it++;
		m_sign = 1;
	}
	if (it == str_number.end()) {
		m_number.push_back(0);		
		return;
	}

	//converting each digit into number
	//if string contains invalid character, returns 0
	for (;it != str_number.end();it++)
	{		
		if (!isdigit(*it))
		{
			m_number.clear();
			m_number.push_back(0);
			m_sign = 1;
			return;
		}
		
		m_number.push_back(*it-'0');		
	}

}


std::string BigNum::to_string()
{
	std::string output;
	if (m_sign == -1)
	{
		output.append("-");
	}

	for (auto &c : m_number)
	{
		output.append(std::to_string(c));
	}
	return output;
}

BigNum::BigNum(const BigNum & other)
{
	*this = other;
}

BigNum & BigNum::operator=(const BigNum & rhs)
{
	//check self-assignment
	if (this != &rhs)
	{
		this->m_number = rhs.m_number;
		this->m_sign = rhs.m_sign;
	}
	return *this;
}


bool BigNum::operator==(const BigNum & rhs) const
{
	if (this->m_sign != rhs.m_sign) return false;
	if (this->m_number.size() != rhs.m_number.size()) return false;

	for (int i = this->m_number.size() - 1; i >= 0; i--)
	{
		if (this->m_number[i] != rhs.m_number[i]) return false;
	}

	return true;
}

bool BigNum::operator!=(const BigNum & rhs) const
{
	return !operator==(rhs);
}

bool BigNum::operator<(const BigNum & rhs) const
{
	return !operator>(rhs) && !operator==(rhs);
}

bool BigNum::operator>(const BigNum & rhs) const
{
	//if true, compared numbers are both positive, if false, both are negative
	bool positive;	

	//check if signs are different
	if (this->m_sign < rhs.m_sign) return false;	
	else if (this->m_sign > rhs.m_sign) return true;	

	//if numbers are both ositive, positive = true, else false
	else
	{
		if (this->m_sign == -1) positive = false;
		else positive = true;
	}

	// if first < second, is assigned as true
	bool ret_val = false;

	//check size
	if (this->m_number.size() > rhs.m_number.size()) ret_val = true;
	else if (this->m_number.size() < rhs.m_number.size()) ret_val = false;

	//compare by digits
	else
	{
		for (int i = this->m_number.size() - 1; i >= 0; i--)
		{
			if (this->m_number[i] > rhs.m_number[i])
			{
				ret_val = true;
				break;
			}
			else if (this->m_number[i] < rhs.m_number[i])
			{
				ret_val = false;
				break;
			}
		}

		if (!ret_val) return false;
	}
	
	//if negative, bigger number is lower
	return !(ret_val^positive);
}

bool BigNum::operator<=(const BigNum & rhs) const
{
	return !operator>(rhs) || operator==(rhs);
}

bool BigNum::operator>=(const BigNum & rhs) const
{
	return operator>(rhs) || operator==(rhs);
}


BigNum BigNum::operator-() const
{
	return BigNum(this->m_number, this->m_sign*(-1));
}

const BigNum & BigNum::operator+() const
{
	return *this;
}


BigNum BigNum::operator+(const BigNum & rhs) const
{
	//if numbers have different signs, do minus operation
	if (this->m_sign != rhs.m_sign)
	{		
		return (*this - (-rhs));		
	}

	//copy numbers
	std::vector<uint8_t> first = this->m_number;
	std::vector<uint8_t> second = rhs.m_number;
	std::vector<uint8_t> out;

	//first number must be shorter
	if (first.size() > second.size())
	{
		first.swap(second);
	}
	std::reverse(first.begin(), first.end());
	std::reverse(second.begin(), second.end());

	//offset from previous iteration
	uint8_t offset = 0;

	for (size_t i = 0; i < first.size(); i++)
	{
		auto sum = first[i] + second[i] + offset;
		out.push_back(sum % 10);
		offset = sum / 10;
	}
	for (size_t i = first.size(); i < second.size(); i++)
	{
		auto sum = second[i] + offset;
		out.push_back(sum % 10);
		offset = sum / 10;
	}
	if (offset) out.push_back(offset);

	std::reverse(out.begin(), out.end());
		
	return BigNum(out, rhs.m_sign);
}

BigNum BigNum::operator-(const BigNum & rhs) const
{
	//if numbers have different signs, do plus operation
	if (this->m_sign != rhs.m_sign)
	{
		return *this + (-rhs);
	}

	std::vector<uint8_t> first = this->m_number;
	std::vector<uint8_t> second = rhs.m_number;
	std::vector<uint8_t> out;

	if (*this == rhs)
	{
		
		return BigNum();
	}

	int out_sign = this->m_sign;
	
	if (abs(*this) < abs(rhs))
	{		
		first.swap(second);
		out_sign *= -1;
	}

	std::reverse(first.begin(), first.end());
	std::reverse(second.begin(), second.end());

	//offset from previous order
	uint8_t offset = 0;

	for (size_t i = 0; i < second.size(); i++)
	{
		int minuend = first[i];
		int subtrahend = second[i] + offset;
		if (subtrahend > minuend) minuend += 10;

		int difference = minuend - subtrahend;		
		out.push_back(difference);

		offset = minuend / 10;
	}
	for (size_t i = second.size(); i < first.size(); i++)
	{
		int minuend = first[i];
		int subtrahend = offset;
		if (subtrahend > minuend) minuend += 10;

		int difference = minuend - subtrahend;
		out.push_back(difference);

		offset = minuend / 10;
	}

	std::vector<uint8_t>::iterator it = out.end()-1;
	while (*it == 0)
	{
		it--;
		out.pop_back();
	}

	std::reverse(out.begin(), out.end());
	return BigNum(out, out_sign);
}

BigNum BigNum::operator*(const BigNum & rhs) const
{
	if (*this == 0 || rhs == 0) return BigNum();

	//copy numbers
	std::vector<uint8_t> first = this->m_number;
	std::vector<uint8_t> second = rhs.m_number;

	
	BigNum out_num;
	

	std::reverse(first.begin(), first.end());
	std::reverse(second.begin(), second.end());

	size_t i = 0;

	for (auto right : second)
	{
		std::vector<uint8_t> out(i, 0);
		uint8_t offset=0;
		for (auto left : first)
		{
			int product = left*right + offset;
			out.push_back(product % 10);
			offset = product / 10;
		}
		if (offset != 0) out.push_back(offset);

		std::reverse(out.begin(), out.end());
		BigNum tmp(out, 1);
		
		out_num += tmp;
		
		i++;
	}
	
	int out_sign;
	if ((this->m_sign + rhs.m_sign) == 0) out_sign = -1;
	else out_sign = 1;
	out_num.m_sign = out_sign;

	return out_num;

}

BigNum & BigNum::operator+=(const BigNum & rhs)
{		
	BigNum out = *this + rhs;
	*this = out;
	return *this;
}

BigNum & BigNum::operator-=(const BigNum & rhs)
{
	BigNum out = *this - rhs;
	*this = out;
	return *this;
}

BigNum & BigNum::operator*=(const BigNum & rhs)
{
	BigNum out = *this * rhs;
	*this = out;
	return *this;
}


BigNum::BigNum(std::vector<uint8_t> number, int sign)
{
	m_number = number;
	m_sign = sign;
}


BigNum abs(const BigNum & num)
{
	return BigNum(num.m_number, 1);
}
