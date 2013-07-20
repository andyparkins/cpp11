#include <iostream>
using namespace std;

// specialisation for doing the work on a single parameter
template<typename ThisT>
ostream &out(ostream &s, ThisT thisParam)
{
	s << thisParam << endl;
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
	out(s, nextParams...);
	return s;
}


// ----


//template<typename... Args>
//inline ostream &lambdaOut(ostream &s, Args &&... args) {
//	auto f = [&]{ s << args << endl, 1; }...;
//}


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
		out( clog, 1, 2.5, "three");
	}

	void testVariadicLambda() {
//		lambdaOut( clog, 1, 2.5, "three");
	}

	// --- Auto-generate suite() convenience function
	CPPUNIT_TEST_SUITE(FunctionalTest);
	CPPUNIT_TEST(testVariadic);
	CPPUNIT_TEST(testVariadicLambda);
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
