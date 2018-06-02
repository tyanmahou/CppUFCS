# CppUFCS
'Uniform Function Call Syntax' for C++17.  
  
  It can unify the **member function** and the **free function**.

# Example

```cpp
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

	//method chain
	{
		int result = (hoge >> func_(1)) + (foo >> func_(2));
		"result is %d\n" >> printf_(result);
	}
	//free
	{
		int result2 = func_(hoge, 3) + func_(foo, 4);
		printf_("result is %d\n", result2);
	}
	return 0;
}
```
# How to
`CppUFCS.hpp` Header Include Only.

# License
MIT

