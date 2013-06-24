#include <iostream>
#include <memory>
using namespace std;


int main()
{
	unique_ptr<int> p1 { new int };
	unique_ptr<int> p2;

	cerr << "p1 = " << (int*)(p1.get()) << endl;
	cerr << "p2 = " << (int*)(p2.get()) << endl;

	// This is illegal, can't copy a unique_ptr
	//   p2 = p1;
	// We can, however, move one
	p2 = std::move(p1);

	cerr << "p1 = " << (int*)(p1.get()) << endl;
	cerr << "p2 = " << (int*)(p2.get()) << endl;

	return 0;
}
