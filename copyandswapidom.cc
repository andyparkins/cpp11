// http://stackoverflow.com/questions/3279543/what-is-the-copy-and-swap-idiom

#include <utility>
#include <cstddef>
#include <iostream>
using namespace std;

class T {
  public:
	// Note that this is the only piece of code that allocates
	// resources, and the destructor is the only place that deallocates
	// resources.
	T(size_t s) :
		resource(s ? new int[s] : nullptr),
		size(s) {
		cerr << "constructor " << __func__ << " " << size << " @ " << resource << endl;
	}

	// "The Big Three", constructor, copy constructor, assignment
	// operator, all need declaring or none.
	T(const T& O) :
		// C++11 lets us call other constructors, we'll use this to
		// allocate our resources
		T(O.size) {
		cerr << "copy constructor " << __func__
			<< " " << size << " @ " << resource
			<< " from " << O.resource << endl;
		// Resource are allocated, but we want to do a deep copy
		copy(O.resource, O.resource + O.size, resource);
	}
	~T() {
		delete[] resource;
	}
	T& operator=(T O) {
		cerr << __func__ << " copying " << O.resource << " from temporary" << endl;
		// Note: non-const non-reference parameter.  This makes the compiler
		// use the copy/move constructor to perform the copy and also
		// allows this single function to act as both a copy assignment
		// and move assignment.  Now called the unified assignment
		// operator

		// The temporary object, O, now holds a deep copy, we therefore
		// have no need to reimplment code that's already in the 
		swap(O);
		// O now contains a pointer to whatever we were using, which
		// will be correctly released using its destructor, meaning we
		// don't have to duplicate destructor code
		return *this;
	}

	// "The Big Four", C++11 adds an extra _must implement_, the move
	// constructor
	T(T&& O) :
		T(0) {
		cerr << "move constructor " << __func__ << endl;
		// First note that C++11 lets us call other constructors, so
		// we need no duplicated code.  Next note that this is very
		// similar to the assignment operator, we have a temporary that
		// will shortly get destructed, that we want to copy.  We'll
		// pull the same stunt then...
		swap(O);
	}

	// ... and a half.  Swap is common code between copy and move
	// constructors and assignment operator
	void swap(T& O) {
		// This function masks the global one unless we unmask it
		using std::swap;
		// Swap our resources with the argument's resources.  Swaps can
		// often be as efficient or nearly as efficient as an
		// assignment.
		swap(resource, O.resource);
		swap(size, O.size);
	}

	int getSize() const { return size; }
	const int *getPointer() const { return resource; }

  protected:
	int *resource;
	int size;
};


int main()
{
	T a(100);
	T b(0);

	// Assignment from an lvalue uses copy constructor because a has to
	// be accessible after the assignment
	b = a;

	// Construction from a temporary (rvalue) should use move constructor,
	// but it uses copy elision to construct c in place from the
	// temporary.
	T c(T(101));
	// Assignment from an rvalue should use move construction
	// should result in:
	//   - construct 102
	//   - move construct temporary parameter
	//   - copy from temporary
	// However, the optimiser notices that there is no need to move a
	// temporary to a temporary so just constructs in place ready for
	// the copy from temporary
	c = T(102);

	cerr << "a.size = " << a.getSize() << " @ " << a.getPointer() << endl;
	cerr << "b.size = " << b.getSize() << " @ " << b.getPointer() << endl;
	cerr << "c.size = " << c.getSize() << " @ " << c.getPointer() << endl;

	return 0;
}
