#include <iostream>
#include <sstream>
#include <string>
using namespace std;

// Provide a do-nothing function that will accept any number of
// arguments
template<typename... Ignore> inline void pass(Ignore&&...) {}


// specialisation for doing the work on a single parameter
template<typename ThisT>
ostream &out(ostream &s, const ThisT &thisParam)
{
	s << thisParam;
	return s;
}

//
// Let's imagine a call,
//   out(s, 1, 2.0, "three");
// That will instantiate an instance of this function...
//   out<int, double, const char *>(s, 1, 2.0, "three");
// This call simply does
//   out<int>(s, 1);
// Then recursively calls itself with that parameter removed.
//   out(s, 2.0, "three");
// Which instantiates
//   out<double, const char *>(s, 2.0, "three");
// which does this
//   out(s, 2.0);
// then calls
//   out(s, "three");
// however, we don't want it to do that because this out<>() will try to
// recursively call itself.  We catch that with a template
// specialisation, that will be chosen in preference.  See above.
//
template<typename ThisT, typename... RestT>
ostream &out(ostream &s, ThisT thisParam, RestT... nextParams)
{
	out(s, thisParam);
	// the "..." expansion operator takes a "..." template parameter and
	// comma separates using the pattern it wraps.
	// out(s, nextParams...);
	// For example, the following would turn the parameters into an
	// initializer list
	//   {nextParams...}
	// This would return the count of params
	//   sizeof... (nextParams)
	// This would call some other function with each
	//   other_func(nextParams)...
	// That last is exactly what we want.  Except we can't do it,
	// because C++ requires ";" between commands, not ",".
	//   out(s, nextParams)...;
	//   -> out(s, p1), out(s, p2), out(s, p3), etc; // fails.
	// we can cheat that by wrapping the expansion in a do-nothing
	// function, then it's arguments can be separated by commas
	pass(out(s, nextParams)...);
	// if we can do that though, why do we need out() at all?  See
	// outInOne below
	return s;
}


// ----

template<typename... Args>
inline ostream &outInOne(ostream &s, Args &&... args) {

	pass( (s << args, 1)... );
	return s;
}


// ----

template<typename... Args>
struct variadicCount {
	static const size_t n;
};
template<typename... Args>
const size_t variadicCount<Args...>::n = sizeof... (Args);


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
		ostringstream ss;
		out( ss, 1, 2.5, "three");
		string expect("12.5three");
		CPPUNIT_ASSERT_EQUAL(expect, ss.str());
	}

	void testVariadicLambda() {
		ostringstream ss;
		outInOne( ss, 1, 2.5, "three");
		string expect("12.5three");
		CPPUNIT_ASSERT_EQUAL(expect, ss.str());
	}

	void testVariadicCount() {
		using c0 = variadicCount<>;
		using c1 = variadicCount<int>;
		using c2 = variadicCount<int, int>;
		using c3 = variadicCount<int, int, int>;
		using c4 = variadicCount<int, int, int, int>;
		CPPUNIT_ASSERT_EQUAL(0U, c0::n);
		CPPUNIT_ASSERT_EQUAL(1U, c1::n);
		CPPUNIT_ASSERT_EQUAL(2U, c2::n);
		CPPUNIT_ASSERT_EQUAL(3U, c3::n);
		CPPUNIT_ASSERT_EQUAL(4U, c4::n);
	}

	// --- Auto-generate suite() convenience function
	CPPUNIT_TEST_SUITE(FunctionalTest);
	CPPUNIT_TEST(testVariadic);
	CPPUNIT_TEST(testVariadicLambda);
	CPPUNIT_TEST(testVariadicCount);
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
