// -------------- Includes
// --- C
// --- C++
#include <iostream>
// --- OS
// --- Project libs


// -------------- Namespace
using namespace std;


// -------------- Macros


// -------------- Module Globals


// -------------- World Globals (need "extern"s in header)


// -------------- Enumerations
enum class eFizzBuzz {
	Null,
	Fizz,
	Buzz,
	FizzBuzz,
};


// -------------- Class declarations


// -------------- Class member static initialisers


// -------------- Class member definitions


// -------------- Function definitions

ostream &operator<<(ostream &s, eFizzBuzz fb)
{
	switch(fb) {
		case eFizzBuzz::Fizz:
			return s << "Fizz";
		case eFizzBuzz::Buzz:
			return s << "Buzz";
		case eFizzBuzz::FizzBuzz:
			return s << "FizzBuzz";
		case eFizzBuzz::Null:
			return s << "Null";
	}
}

eFizzBuzz FizzBuzz(int n)
{
	bool f = (n % 3) == 0;
	bool b = (n % 5) == 0;
	if(f && b)
		return eFizzBuzz::FizzBuzz;
	if(f)
		return eFizzBuzz::Fizz;
	if(b)
		return eFizzBuzz::Buzz;
	return eFizzBuzz::Null;
}

ostream &FizzBuzz(ostream &s, int n)
{
	for(int i = 0; i <= n; i++) {
		auto fb = FizzBuzz(i);
		if(fb == eFizzBuzz::Null) {
			s << i << endl;
		} else {
			s << fb << endl;
		}
	}
	return s;
}


// -------------- Unit test
#ifdef UNITTEST
#include <cppunit/extensions/HelperMacros.h>
#include <stdexcept>
#include <sstream>
#include <limits>

//
// Class:	FizzBuzzTest
// Description:
//  A TestFixture groups multiple tests in a common environment; we
//  assume that the common environment is costly or inconvenient to
//  manufacture, so we do multiple tests with that one environment.
//
class FizzBuzzTest : public CppUnit::TestFixture
{
  public:
	// Construct test environment
	void setUp(void) {}
	// Destruct test environment
	void tearDown(void) {}

	void test_fizzbuzz_operator() {
		ostringstream null, fizz, buzz, fizzbuzz;
		null << eFizzBuzz::Null;
		CPPUNIT_ASSERT_EQUAL(string("Null"), null.str());
		fizz << eFizzBuzz::Fizz;
		CPPUNIT_ASSERT_EQUAL(string("Fizz"), fizz.str());
		buzz << eFizzBuzz::Buzz;
		CPPUNIT_ASSERT_EQUAL(string("Buzz"), buzz.str());
		fizzbuzz << eFizzBuzz::FizzBuzz;
		CPPUNIT_ASSERT_EQUAL(string("FizzBuzz"), fizzbuzz.str());
	}

	void test_fizzbuzz() {
		for( int i = -100*15; i < 100*15; i += 15 ) {
			CPPUNIT_ASSERT_EQUAL(eFizzBuzz::FizzBuzz, FizzBuzz(i + 0));
			CPPUNIT_ASSERT_EQUAL(eFizzBuzz::Null, FizzBuzz(i + 1));
			CPPUNIT_ASSERT_EQUAL(eFizzBuzz::Null, FizzBuzz(i + 2));
			CPPUNIT_ASSERT_EQUAL(eFizzBuzz::Fizz, FizzBuzz(i + 3));
			CPPUNIT_ASSERT_EQUAL(eFizzBuzz::Null, FizzBuzz(i + 4));
			CPPUNIT_ASSERT_EQUAL(eFizzBuzz::Buzz, FizzBuzz(i + 5));
			CPPUNIT_ASSERT_EQUAL(eFizzBuzz::Fizz, FizzBuzz(i + 6));
			CPPUNIT_ASSERT_EQUAL(eFizzBuzz::Null, FizzBuzz(i + 7));
			CPPUNIT_ASSERT_EQUAL(eFizzBuzz::Null, FizzBuzz(i + 8));
			CPPUNIT_ASSERT_EQUAL(eFizzBuzz::Fizz, FizzBuzz(i + 9));
			CPPUNIT_ASSERT_EQUAL(eFizzBuzz::Buzz, FizzBuzz(i + 10));
			CPPUNIT_ASSERT_EQUAL(eFizzBuzz::Null, FizzBuzz(i + 11));
			CPPUNIT_ASSERT_EQUAL(eFizzBuzz::Fizz, FizzBuzz(i + 12));
			CPPUNIT_ASSERT_EQUAL(eFizzBuzz::Null, FizzBuzz(i + 13));
			CPPUNIT_ASSERT_EQUAL(eFizzBuzz::Null, FizzBuzz(i + 14));
		}

		int digits = numeric_limits<int>::digits;
		int max = numeric_limits<int>::max();
		int min = numeric_limits<int>::min();
		switch( digits ) {
			// max = (2^n-1) ; min = -(2^n)
			// All happen to be identically, Null
			case 15:
			case 23:
			case 31:
			case 63:
				CPPUNIT_ASSERT_EQUAL(eFizzBuzz::Null, FizzBuzz(min));
				CPPUNIT_ASSERT_EQUAL(eFizzBuzz::Null, FizzBuzz(max));
				break;
			default:
				CPPUNIT_ASSERT_EQUAL(0, digits);
				throw logic_error("unsupported architecture");
		}
	}

	// -----------------------

	// HelperMacros to manufacture `static CppUnit::Test *suite()`
	CPPUNIT_TEST_SUITE(FizzBuzzTest);
	CPPUNIT_TEST(test_fizzbuzz_operator);
	CPPUNIT_TEST(test_fizzbuzz);
	CPPUNIT_TEST_SUITE_END();
};
// HelperMacro to auto-call FizzBuzzTest::suite() and add it to a
// single registry
CPPUNIT_TEST_SUITE_REGISTRATION(FizzBuzzTest);


// -------------- main()
#ifndef UNITTESTALL
// --- cppunit run includes
#include <cppunit/ui/text/TestRunner.h>
#include <cppunit/TextOutputter.h>
// ---
#include <iostream>
#include <stdexcept>
int main()
{
	// --- Boilerplate cppunit code
	// Set up runner to run all test in registry
	CppUnit::TextUi::TestRunner runner;
	runner.addTest( CppUnit::TestFactoryRegistry::getRegistry().makeTest() );
	// Redirect output to clog,
	runner.setOutputter(new CppUnit::TextOutputter(&runner.result(), std::clog));
	// Run all and give success indiciation
	return runner.run() ? 0 : 1;
}
#endif
#endif

