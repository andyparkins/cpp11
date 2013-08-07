// -------------- Includes
// --- C
// --- C++
// --- OS
// --- Project libs


// -------------- Namespace


// -------------- Macros
#define UNITTEST


// -------------- Module Globals


// -------------- World Globals (need "extern"s in header)


// -------------- Class member static initialisers


// -------------- Enumerations
enum class eColorClass { red, green, blue };
enum eColor { red, green, blue };


// -------------- Class member definitions


// -------------- Function definitions


// -------------- Unit test
#ifdef UNITTEST
#include <iostream>
using namespace std;
ostream &operator<<(ostream &s, eColorClass F) {
	switch(F) {
		case eColorClass::red:
			return s << "red";
		case eColorClass::green:
			return s << "green";
		case eColorClass::blue:
			return s << "blue";
	}
}


#include <cppunit/extensions/HelperMacros.h>
//
// Class:	EnumerationTest
// Description:
//  A TestFixture groups multiple tests in a common environment; we
//  assume that the common environment is costly or inconvenient to
//  manufacture, so we do multiple tests with that one environment.
//
class EnumerationTest : public CppUnit::TestFixture
{
  public:
	// Construct test environment
	void setUp(void) {}
	// Destruct test environment
	void tearDown(void) {}

	void enumerations()
	{
		// C-style enumerations are just numbers, and don't have scope
		int c = red;
		// The following will not work, class enumerations have strict type
		//   int c2 = eColorClass::red;

		CPPUNIT_ASSERT(red == c);
		CPPUNIT_ASSERT(red != green);
		CPPUNIT_ASSERT(red != blue);
		CPPUNIT_ASSERT(blue != green);
	}

	// -----------------------

	// HelperMacros to manufacture `static CppUnit::Test *suite()`
	CPPUNIT_TEST_SUITE(EnumerationTest);
	CPPUNIT_TEST(enumerations);
	CPPUNIT_TEST_SUITE_END();
};
// HelperMacro to auto-call EnumerationTest::suite() and add it to a
// single registry
CPPUNIT_TEST_SUITE_REGISTRATION(EnumerationTest);


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
	try {
		return runner.run() ? 0 : 1;
	} catch( std::exception &e ) {
		std::clog << "EXCEPTION: " << e.what() << std::endl;
		return 254;
	} catch( ... ) {
		std::clog << "EXCEPTION" << std::endl;
		return 255;
	}
}
#endif
#endif

