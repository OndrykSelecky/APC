/*operator =  - treba kontrolovat self assignment
productivity power tool

operatory pomocou std::rel_ops;

*/
class A
{
public:

	A& operator+=(const A& rhs)
	{

	}

	A operator+(const A& rhs) const
	{

		A tmp(*this);
		tmp += rhs;
		return tmp;
	}

	friend A operator-(const A &lhs, const A &rhs);

};

A operator-(const A &lhs, const A &rhs) 
{
	A tmp(lhs);
	tmp += rhs;
	return tmp;

}

//trik:

A operator-(A lhs, const A &rhs)
{
	lhs += rhs;
	return lhs;

}


int main(int argc, char **argv)
{

	return 0;
}