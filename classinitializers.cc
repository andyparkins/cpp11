#include <iostream>
#include <string>
using namespace std;

class SomeOtherClass {
  public:
	virtual const string &getName() const = 0;
};

class SomeClass : public SomeOtherClass {
  public:
	virtual const string &getName() const {
		return NAME;
	}
  private:
	// C++11 allows us to initialise members inline ... unfortunately
	// not statics
	const string NAME = "SomeClass";
};

int main()
{
	SomeClass x;
	string name(x.getName());
	cerr << "Before..." << endl;
	cerr << "x.getName() = " << x.getName() << endl;
	cerr << "name        = " << name << endl;

	cerr << "Altering copy" << endl;
	name[0] = 's';
	cerr << "name = " << name << endl;
	cerr << "x.getName() = " << x.getName() << endl;
	cerr << "name        = " << name << endl;

	return 0;
}
