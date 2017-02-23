#pragma once
//v .h includujem co najmenej
#include <string>

class Foo 
{
public:
	Foo();
	~Foo();
	int GetI() const //neviem menit nic vo funkcii 
	{
		this;
		
		return m_i; 
	}

	const std::string& GetString() const  //ked neupravim potom
	{
		return m_s;
	}

	

private:
	int m_i = 0;
	std::string m_s;

};
