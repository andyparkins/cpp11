#include <iostream>
#include <memory>
using namespace std;

class TTag
{
  public:
	TTag() = default;
	virtual ~TTag() = default;
	virtual const char *className() const { return "TTag"; }

	virtual unique_ptr<TTag> clone() = 0;
};

class TTag0 : public TTag
{
  public:
	TTag0() = default;
	const char *className() const override { return "TTag0"; }

	// It's important to note that what we're doing here only works
	// because C++11 supports move semantics for return-by-value.  If it
	// didn't this would try to use the copy constructor of
	// unique_ptr<> -- there isn't one; by definition you can't copy a
	// unique_ptr.  What we're telling the caller with this is that they
	// are now the owner of the pointer; it's their responsibility to
	// free the memory (although unique_ptr<> will take care of that for
	// them)
	unique_ptr<TTag> clone() { return unique_ptr<TTag>(new TTag0(*this)); }
};


int main()
{
	unique_ptr<TTag> original(new TTag0());
	unique_ptr<TTag> copy { original->clone() };

	clog << "original = " << original->className() << endl;
	clog << "copy     = " << copy->className() << endl;

	return 0;
}
