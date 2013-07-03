#include <iostream>
using namespace std;

// ---- Library

//
// Struct:	IBurnable
// Description:
//  This is the interface that burnable objects must implement to make
//  use of our "library".  It defines the contract that an object must
//  agree to in order that it be burnable.
//
struct IBurnable
{
	virtual void burn() = 0;
};

//
// Function:	burn
// Description:
//  This stub function represents the library, it's obviously trivial,
//  but it need not be, and the interface can be as complicated as the
//  library requires.
//
void burn(IBurnable &I)
{
	I.burn();
}

// ---- Application

// A book is not a log
class Book
{
  public:
	Book() : iBurnable(*this) {}
	IBurnable &burnable() { return iBurnable; }

	void commitCrime() {
		cout << "A book is on fire" << endl;
	}

  private:
	struct IBurnableBook : public IBurnable {
		IBurnableBook(Book &B) : mBook(B) {}
		void burn() override { mBook.commitCrime(); }
		Book &mBook;
	} iBurnable;
};

// A log is not a book
class Log
{
  public:
	Log() : iBurnable(*this) {}
	IBurnable &burnable() { return iBurnable; }

	void makeWarmth() {
		cout << "A log is on fire" << endl;
	}

  private:
	struct IBurnableLog : public IBurnable {
		IBurnableLog(Log &B) : mLog(B) {}
		void burn() override { mLog.makeWarmth(); }
		Log &mLog;
	} iBurnable;
};

// --------

int main()
{
	Log L;
	Book B;

	// Call L.makeWarmth() via an interface
	burn(L.burnable());
	// Call B.commitCrime() via an interface
	burn(B.burnable());

	return 0;
}
