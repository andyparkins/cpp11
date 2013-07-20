#include <iostream>
#include <stdexcept>
using namespace std;

ostream &iosprintf(ostream &s, const char *fmt)
{
	while (*fmt) {
		if (*fmt == '%') {
			if (*(fmt + 1) == '%') {
				++fmt;
			} else {
				throw std::runtime_error("invalid format string: missing arguments");
			}
		}
		s << *fmt++;
	}
	return s;
}

// The template definition is the most important part, this template
// requires at least one parameter, T.  That will be inferred from the
// parameters passed to iosprintf().
//
// Let's say we begin with 
template<typename T, typename... Args>
ostream &iosprintf(ostream &s, const char *fmt, T value, Args... args)
{
	// Iterate throught the format string
	while (*fmt) {
		// ... until we find an escape character
		if (*fmt == '%') {
			// Skip the '%'s
			++fmt;
			// ... when we do, handle the next character carefully
			if( *fmt == '\0') {
				throw std::logic_error("% was last character in format string");
			} else if (*fmt == '%') {
				// No special handling, drop through
			} else if( *fmt == 'x' || *fmt == 'X') {
				s << hex << value << dec;
			} else {
				// we should convert the full printf() format parameters
				// into stream manipulators here.
				// type-retaining value output
				s << value;
				// call even when *fmt == 0 to detect extra arguments
				return iosprintf(s, fmt + 1, args...); 
			}
		}
		s << *fmt++;
	}
	throw std::logic_error("extra arguments provided to printf");
}


//#ifdef UNITTEST
#include <stdexcept>
#include <iostream>
// --- cppunit
#include <cppunit/TestFixture.h>
#include <cppunit/TestSuite.h>
#include <cppunit/extensions/HelperMacros.h>
// Runner
#include <cppunit/ui/text/TestRunner.h>
#include <cppunit/TextOutputter.h>

// ----

class FunctionalTest : public CppUnit::TestFixture
{
  public:
	// --- Prepare
	void setUp() { }
	void tearDown() { }

	// --- Tests
	void testVariadic() {
		printf("%u %x %f \"%s\"\n", 1, 0xff, 2.0, "three");
	}

	// --- Auto-generate suite() convenience function
	CPPUNIT_TEST_SUITE(FunctionalTest);
	CPPUNIT_TEST(testVariadic);
	CPPUNIT_TEST_SUITE_END();
};
// Add result of AxiomsTest::suite() to test registry
CPPUNIT_TEST_SUITE_REGISTRATION( FunctionalTest );


// -------------- main()
//#ifndef UNITTESTALL
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
//#endif
//#endif
