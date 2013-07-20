#include <iostream>
#include <vector>
using namespace std;


class Obj {
  public:
	Obj(initializer_list<int> ilist) {
		for( auto &l : ilist )
			mStore.push_back(l);
	}

	int operator[](int i) {
		return mStore[i];
	}

  protected:
	vector<int> mStore;
};


int main()
{
	Obj x {1,2,3,4};

	cerr << "x = " << x[0] << endl;
	cerr << "x = " << x[1] << endl;
	cerr << "x = " << x[2] << endl;
	cerr << "x = " << x[3] << endl;

	return 0;
}
