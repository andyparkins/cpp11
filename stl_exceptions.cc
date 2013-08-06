// -------------- Includes
// --- C
// --- C++
#include <string>
#include <system_error>
// --- OS
// --- Project libs


// -------------- Namespace
using namespace std;


// -------------- Macros
#define UNITTEST


// -------------- Module Globals


// -------------- World Globals (need "extern"s in header)


// -------------- Enumerations
enum class mylib_error_code {
	something_wicked,
	apocalypse_now,
};


// -------------- Class declarations

// http://blog.think-async.com/2010/04/system-error-support-in-c0x-part-4.html
class mylib_category_impl : public error_category {
  public:
	const char *name() const noexcept override { return "mylib"; }
	string message(int ev) const override {
		switch(static_cast<mylib_error_code>(ev)) {
			case mylib_error_code::something_wicked:
				return "Something wicked";
			case mylib_error_code::apocalypse_now:
				return "Apocalypse now";
			default:
				throw logic_error("mylib_category unknown error");
		}
	}
};

const error_category &mylib_category() {
	static mylib_category_impl mylib_category_instance;
	return mylib_category_instance;
}

std::error_code make_error_code(mylib_error_code e)
{
	return std::error_code(
			static_cast<int>(e),
			mylib_category());
}

std::error_condition make_error_condition(mylib_error_code e)
{
	return std::error_condition(
			static_cast<int>(e),
			mylib_category());
}

template <>
struct is_error_code_enum<mylib_error_code>
	: public true_type {};




// -------------- Class member static initialisers


// -------------- Class member definitions


// -------------- Function definitions


// -------------- Unit test
#ifdef UNITTEST
#include <cppunit/extensions/HelperMacros.h>

//
// Class:	ExceptionTest
// Description:
//  A TestFixture groups multiple tests in a common environment; we
//  assume that the common environment is costly or inconvenient to
//  manufacture, so we do multiple tests with that one environment.
//
class ExceptionTest : public CppUnit::TestFixture
{
  public:
	// Construct test environment
	void setUp(void) {}
	// Destruct test environment
	void tearDown(void) {}

	void test_system_error() {
		for(int i = 0; i < 134; i++) {
			try {
				throw system_error(i, system_category());
			} catch( const exception &e ) {
				clog << e.what() << "\r";
			}
		}
	}

	void test_mylib_error() {
		clog << endl;
		for(int i = 0; i < 2; i++) {
			try {
				throw system_error(i, mylib_category());
			} catch( const exception &e ) {
				clog << "mylib: " << e.what() << endl;
			}
		}
	}

	// -----------------------

	// HelperMacros to manufacture `static CppUnit::Test *suite()`
	CPPUNIT_TEST_SUITE(ExceptionTest);
	CPPUNIT_TEST(test_system_error);
	CPPUNIT_TEST(test_mylib_error);
	CPPUNIT_TEST_SUITE_END();
};
// HelperMacro to auto-call ExceptionTest::suite() and add it to a
// single registry
CPPUNIT_TEST_SUITE_REGISTRATION(ExceptionTest);


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

