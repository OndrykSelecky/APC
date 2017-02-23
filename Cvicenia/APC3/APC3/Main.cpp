


/*
- vracanie cez referenciu - nie príliš šastné, radšej vraca hodnoty (napr. v pair)
- chytanie vınimky - referencia
- .at = checkne hranice, hodí vınimku ([] neskontrolujú)


*/
#include <iostream>
#include <memory>

class A {
public:
	virtual ~A() {};
	A() {};
	
	A(const A& other)
	{
		//copy constructor
		//nesmú mat zmeny, ani vıpisy
		std::cout << "Copy" << std::endl;
	}

	A& operator=(const A& rhs)
	{
		std::cout << "Operator";
		return *this;
	}



	virtual void print()
	{
		std::cout << "A";
	}

};

class B : public A 
{
public:
	~B() override {};
	virtual void print() override
	{
		std::cout << "B";
	}
};

/*void f(const A* a)
{
	const B* pb = dynamic_cast<const B*>(a);	//umoznuje zistit typ pointra, ak nie je a typom B, vráti 0
												//referencie: 
	if (pb == nullptr)
	{
		std::cout << "A";
	}
	else 
	{
		std::cout << "B";
	}
}*/

void f2(A a)
{
	a.print();
}

int main(int argc, char **argv)
{
	// v poriadku
	B b;
	A *a = &b;

	//opaène to fungova nebude
	//B *pb = dynamic_cast<B*>(a); // triedy musia ma virt. metódy

	
	std::unique_ptr<A> a_ptr (new B);
	//f2(std::move(a_ptr));
	A a1;
	//f(&a1);

	B b1;
	//f(&b1);

	a1 = b1;
	A c;
	A k;
	k = c;


	return 0;
}