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
	static const string NAME;
};
const string SomeClass::NAME = "SomeClass";

int main()
{
	SomeClass x;
	string name(x.getName());
	cerr << "name = " << name << endl;
	name[0] = 's';
	cerr << "name = " << name << endl;

	return 0;
}
