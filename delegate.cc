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

// ---------

// This is a simulation of a really unpleasant callback; the library
// writer hasn't given us an opaque pointer.
extern "C" int bare_instant_callback( int (*cb)(void*) );

template<int(*UnopaqueFunction)(void*), void *Opaque>
class bare_wrapping_callback {
  public:
	static int callback_with_opaque() {
		return UnopaqueFunction(Opaque);
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

	void testBareCallback() {
	// Doesn't work: &mFunctor is not a constant expression
//		typedef functor_as_c_function<decltype(mFunctor)> cf;
//		typedef bare_wrapping_callback<cf::c_function, &mFunctor> bcf;
//		clog << bare_instant_callback(bcf) << endl;
	}

	// --- Auto-generate suite() convenience function
	CPPUNIT_TEST_SUITE(FunctionalTest);
	CPPUNIT_TEST(testCallback);
	CPPUNIT_TEST(testBareCallback);
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
	return runner.run() ? 0 : 1;
}
//#endif
//#endif
