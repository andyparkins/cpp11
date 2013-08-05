#include <iostream>
#include <memory>
#include <stdexcept>
using namespace std;


class Obj {
  public:
	Obj() {
		throw logic_error("Purposefully throw during construction");
	}
	~Obj() {
		DestructorCalled = true;
	}

	static bool DestructorCalled;
};
bool Obj::DestructorCalled = false;


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

class ObjectTest : public CppUnit::TestFixture
{
  public:
	// --- Prepare
	void setUp() { }
	void tearDown() { }

	// --- Tests
	void testConstruction() {
		Obj::DestructorCalled = false;
		CPPUNIT_ASSERT_THROW(Obj x, logic_error);
		// FACT: Destructors are not called when a constructor throws
		CPPUNIT_ASSERT_EQUAL(false, Obj::DestructorCalled);
	}

	// --- Auto-generate suite() convenience function
	CPPUNIT_TEST_SUITE(ObjectTest);
	CPPUNIT_TEST(testConstruction);
	CPPUNIT_TEST_SUITE_END();
};
// Add result of AxiomsTest::suite() to test registry
CPPUNIT_TEST_SUITE_REGISTRATION(ObjectTest);


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
