#include <iostream>

using namespace std;

constexpr int square(int v) { return v*v; }

// Evaluated at compile time
constexpr int x = square(10);
// Evaluated at runtime
const int y = square(10);


int main()
{
	cout
		<< "square(10)  = " << square(10) << endl
		<< "constexpr x = " << x << endl
		<< "const y     = " << y << endl;

	return 0;
}
