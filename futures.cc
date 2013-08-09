// -------------- Includes
// --- C
// --- C++
#include <future>
#include <thread>
#include <chrono>
#include <iostream>
// --- OS
// --- Project libs


// -------------- Namespace
using namespace std;


// -------------- Macros
#define UNITTEST


// -------------- Module Globals


// -------------- World Globals (need "extern"s in header)


// -------------- Class member static initialisers


// -------------- Class member definitions


// -------------- Function definitions

// -------------- Unit test
#ifdef UNITTEST
ostream &operator<<(ostream &s, future_status F) {
	switch(F) {
		case future_status::deferred:
			return s << "deferred";
		case future_status::ready:
			return s << "ready";
		case future_status::timeout:
			return s << "timeout";
	}
}

#include <cppunit/extensions/HelperMacros.h>

//
// Class:	FutureTest
// Description:
//  A TestFixture groups multiple tests in a common environment; we
//  assume that the common environment is costly or inconvenient to
//  manufacture, so we do multiple tests with that one environment.
//
class FutureTest : public CppUnit::TestFixture
{
  public:
	// Construct test environment
	void setUp(void) {}
	// Destruct test environment
	void tearDown(void) {}

	void future_from_packaged_task() {
		packaged_task<int()> task([](){ return 7; });
		future<int> f1 = task.get_future();

		CPPUNIT_ASSERT_EQUAL(true, f1.valid());
//		CPPUNIT_ASSERT_EQUAL(false, f1.has_value());
//		CPPUNIT_ASSERT_EQUAL(false, f1.has_exception());

		// launch on a thread
		thread(move(task)).detach();

//		CPPUNIT_ASSERT_EQUAL(true, f1.has_value());
//		CPPUNIT_ASSERT_EQUAL(false, f1.has_exception());
		CPPUNIT_ASSERT_EQUAL(7, f1.get());
	}

	void future_from_async() {
		future<int> f2 = async(launch::async, [](){ return 8; });

		CPPUNIT_ASSERT_EQUAL(true, f2.valid());
//		CPPUNIT_ASSERT_EQUAL(future_status::timeout,
//				f2.wait_for(chrono::seconds(0)));

		CPPUNIT_ASSERT_EQUAL(8, f2.get());
	}

	void future_from_promise() {
		promise<int> p;
		future<int> f3 = p.get_future();

		CPPUNIT_ASSERT_EQUAL(true, f3.valid());
//		CPPUNIT_ASSERT_EQUAL(future_status::timeout,
//				f3.wait_for(chrono::seconds(0)));

		thread( [](promise<int>& p){
				p.set_value(9);
			},
			ref(p) ).detach();

		CPPUNIT_ASSERT_EQUAL(9, f3.get());
	}

	// -----------------------

	// HelperMacros to manufacture `static CppUnit::Test *suite()`
	CPPUNIT_TEST_SUITE(FutureTest);
	CPPUNIT_TEST(future_from_packaged_task);
	CPPUNIT_TEST(future_from_async);
	CPPUNIT_TEST(future_from_promise);
	CPPUNIT_TEST_SUITE_END();
};
// HelperMacro to auto-call FutureTest::suite() and add it to a
// single registry
CPPUNIT_TEST_SUITE_REGISTRATION(FutureTest);


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

