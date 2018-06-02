#include<iostream>
#include<cstdio>
#include<CppUFCS.hpp>

struct Hoge
{
	int func(int a)
	{
		return a + 2;
	}
};
struct Foo
{};

int func(Foo foo, int a)
{
	return a * 2;
}

namespace UFCS
{
	USE_UFCS(func);

	//If a free function is defined in the namespace 
	USE_UFCS(printf, std::);
}

int main()
{
	using namespace UFCS;
	Hoge hoge;
	Foo foo;

	int result = (hoge >> func_(1)) + (foo >> func_(2));

	"result is %d\n" >> printf_(result);

	return 0;
}