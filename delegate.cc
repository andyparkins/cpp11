#include <functional>
using namespace std;

// Want a C-style function-pointer-accepting function
extern "C" int instant_callback( int (*cb)(void *), void *opaque) {
	// A real library would provide a function that let the caller set a
	// function and pointer to be used as a callback in response to some
	// event.  We're just testing, so rather than noting the callback
	// and opaque pointer for later, we just call it instantly
	return cb(opaque);
}

template<typename F>
class functor_as_c_function {
	using R = typename F::result_type;
  public:
	static R c_function( void *opaque ) {
		F &transparent = *(reinterpret_cast<F*>(opaque));
		return transparent();
	}
};


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
	function<int()> mFunctor;
	int x = 10;

  public:
	// --- Prepare
	void setUp() {
		// Lambdas are the easiest functors to create
		mFunctor = [this](){
			return x;
		};
	}
	void tearDown() { }

	// --- Tests
	void testCallback() {
		typedef functor_as_c_function<decltype(mFunctor)> cf;
		CPPUNIT_ASSERT_EQUAL(10, instant_callback(cf::c_function, &mFunctor));
		x = 11;
		CPPUNIT_ASSERT_EQUAL(11, instant_callback(cf::c_function, &mFunctor));
	}

	// --- Auto-generate suite() convenience function
	CPPUNIT_TEST_SUITE(FunctionalTest);
	CPPUNIT_TEST(testCallback);
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
