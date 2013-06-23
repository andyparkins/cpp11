#include <iostream>
using namespace std;

enum class eColorClass { red, green, blue };
enum eColor { red, green, blue };


int main()
{
	// C-style enumerations are just numbers, and don't have scope
	int c = red;
	// The following will not work, class enumerations have strict type
	//   int c2 = eColorClass::red;

	return 0;
}
