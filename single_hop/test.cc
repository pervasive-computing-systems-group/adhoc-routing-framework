#include "test.h"

// Very cheap testing framework
using namespace std;

void test(bool condition, string desc)
{
	if (condition)
		cout << GREEN << "[PASS]: " << desc << END;
	else
		cout << RED << "[FAIL]: " << desc << END;
}

void test_test()
{
	test(true == true, "true == true");
}

int main (int argc, char *argv[])
{
	cout << "[TESTS][SINGLE-HOP]: Running tests..." << endl;

	test_test();

	cout << "[TESTS][SINGLE-HOP]: TESTS COMPLETE." << endl;

	return 0;
}
