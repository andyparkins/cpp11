#include <iostream>
using namespace std;

struct example_struct
{
	int m = reinterpret_cast<int>(this);
};

class example_smart_pointer
{
  public:
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

//
// Class: PointerTest
// Description:
//
template <typename T, typename P>
class PointerTest : public CppUnit::TestFixture
{
  public:
	// --- Prepare
	void setUp() {}
	void tearDown() { }

	// --- Tests
	void testPointerEquivalence() {
		T p = new P;

		// This should hold true for pointers
		CPPUNIT_ASSERT_EQUAL_MESSAGE("Pointer should be equal to pointer to first element",
				p, &p[0]);
		CPPUNIT_ASSERT_EQUAL_MESSAGE("Pointer reference operator should equal pointer dereference",
				p->m, (*p).m);
	}

	// --- Auto-generate suite() convenience function
	typedef PointerTest<T,P> SelfT;
	CPPUNIT_TEST_SUITE(SelfT);
	CPPUNIT_TEST(testPointerEquivalence);
	CPPUNIT_TEST_SUITE_END();
};
typedef PointerTest<example_struct*, example_struct> TestClass;
CPPUNIT_TEST_SUITE_REGISTRATION( TestClass );

//
// Class: BooleanTest
// Description:
//
template <typename T>
class BooleanTest : public CppUnit::TestFixture
{
  public:
	// --- Prepare
	void setUp() {}
	void tearDown() { }

	// --- Tests
	void testBooleanEquivalence() {
		T b1;
		T b2;
	}

	// --- Auto-generate suite() convenience function
	typedef BooleanTest<T> SelfT;
	CPPUNIT_TEST_SUITE(SelfT);
	CPPUNIT_TEST(testBooleanEquivalence);
	CPPUNIT_TEST_SUITE_END();
};
typedef BooleanTest<example_struct> BooleanTestClass;
CPPUNIT_TEST_SUITE_REGISTRATION( BooleanTestClass );


// -------------- main()

int main()
{
	// --- Boilerplate cppunit code
	// Set up runner to run all test in registry
	CppUnit::TextUi::TestRunner runner;
	CppUnit::TestFactoryRegistry &registry = CppUnit::TestFactoryRegistry::getRegistry();
	runner.addTest( registry.makeTest() );
	// Redirect output to clog,
	runner.setOutputter(new CppUnit::TextOutputter(&runner.result(), std::clog));
	// Run all and give success indiciation
	return runner.run("", false) ? 0 : 1;
}
//#endif

