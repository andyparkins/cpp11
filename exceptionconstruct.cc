#include <iostream>
using namespace std;


class verbose_object
{
  public:
	verbose_object() : defaultConstructed(true) {}
	verbose_object(const verbose_object&) : copyConstructed(true) {}
	verbose_object(verbose_object&&) : moveConstructed(true) {}
	~verbose_object() {if(destructed != nullptr) *destructed = true;}
	verbose_object &operator=(const verbose_object&) { copyAssigned = true; return *this; }
	verbose_object &operator=(verbose_object&&) { moveAssigned = true; return *this; }

	bool defaultConstructed = false;
	bool copyConstructed = false;
	bool moveConstructed = false;
	bool copyAssigned = false;
	bool moveAssigned = false;
	mutable bool *destructed = nullptr;
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

class ExceptionTest : public CppUnit::TestFixture
{
  public:
	// --- Prepare
	void setUp() { }
	void tearDown() { }

	// --- Tests
	void testDefaultConstructor() {
		verbose_object O;
		CPPUNIT_ASSERT_MESSAGE("Default constructed should have defaultConstructed", O.defaultConstructed);
		CPPUNIT_ASSERT_MESSAGE("Default constructed should have !copyConstructed", !O.copyConstructed);
		CPPUNIT_ASSERT_MESSAGE("Default constructed should have !moveConstructed", !O.moveConstructed);
		CPPUNIT_ASSERT_MESSAGE("Default constructed should have !copyAssigned", !O.copyAssigned);
		CPPUNIT_ASSERT_MESSAGE("Default constructed should have !moveAssigned", !O.moveAssigned);
	}
	void testDestructor() {
		bool destructed = false;
		{
			verbose_object O;
			CPPUNIT_ASSERT_MESSAGE("Constructed object should have !destructed", !destructed);
			O.destructed = &destructed;
		}
		CPPUNIT_ASSERT_MESSAGE("Destructed object should have destructed", destructed);
	}
	void testCopyConstructor() {
		verbose_object S;
		verbose_object O(S);
		CPPUNIT_ASSERT_MESSAGE("Copy constructed should have !defaultConstructed", !O.defaultConstructed);
		CPPUNIT_ASSERT_MESSAGE("Copy constructed should have copyConstructed", O.copyConstructed);
		CPPUNIT_ASSERT_MESSAGE("Copy constructed should have !moveConstructed", !O.moveConstructed);
		CPPUNIT_ASSERT_MESSAGE("Copy constructed should have !copyAssigned", !O.copyAssigned);
		CPPUNIT_ASSERT_MESSAGE("Copy constructed should have !moveAssigned", !O.moveAssigned);
	}
	void testMoveConstructor() {
		verbose_object S;
		verbose_object O(std::move(S));
		CPPUNIT_ASSERT_MESSAGE("Move constructed should have !defaultConstructed", !O.defaultConstructed);
		CPPUNIT_ASSERT_MESSAGE("Move constructed should have !copyConstructed", !O.copyConstructed);
		CPPUNIT_ASSERT_MESSAGE("Move constructed should have moveConstructed", O.moveConstructed);
		CPPUNIT_ASSERT_MESSAGE("Move constructed should have !copyAssigned", !O.copyAssigned);
		CPPUNIT_ASSERT_MESSAGE("Move constructed should have !moveAssigned", !O.moveAssigned);
	}
	void testCopyAssignment() {
		verbose_object S;
		verbose_object O;
		O = S;
		CPPUNIT_ASSERT_MESSAGE("Move constructed should have defaultConstructed", O.defaultConstructed);
		CPPUNIT_ASSERT_MESSAGE("Move constructed should have !copyConstructed", !O.copyConstructed);
		CPPUNIT_ASSERT_MESSAGE("Move constructed should have !moveConstructed", !O.moveConstructed);
		CPPUNIT_ASSERT_MESSAGE("Move constructed should have copyAssigned", O.copyAssigned);
		CPPUNIT_ASSERT_MESSAGE("Move constructed should have !moveAssigned", !O.moveAssigned);
	}
	void testMoveAssignment() {
		verbose_object O;
		O = verbose_object();
		CPPUNIT_ASSERT_MESSAGE("Move assigned should have defaultConstructed", O.defaultConstructed);
		CPPUNIT_ASSERT_MESSAGE("Move assigned should have !copyConstructed", !O.copyConstructed);
		CPPUNIT_ASSERT_MESSAGE("Move assigned should have !moveConstructed", !O.moveConstructed);
		CPPUNIT_ASSERT_MESSAGE("Move assigned should have !copyAssigned", !O.copyAssigned);
		CPPUNIT_ASSERT_MESSAGE("Move assigned should have moveAssigned", O.moveAssigned);
	}
	void testDefaultedException() {
		bool destructed = false;
		try {
			throw verbose_object();
		} catch( const verbose_object &E ) {
			E.destructed = &destructed;
			CPPUNIT_ASSERT_MESSAGE("Thrown reference should have defaultConstructed", E.defaultConstructed);
			CPPUNIT_ASSERT_MESSAGE("Thrown reference should have !copyConstructed", !E.copyConstructed);
			CPPUNIT_ASSERT_MESSAGE("Thrown reference should have !moveConstructed", !E.moveConstructed);
		}
		CPPUNIT_ASSERT_MESSAGE("Exception should be destructed out of scope", destructed );
	}
	void testCopiedException() {
		verbose_object O;
		try {
			// Copy constructs, then throws a reference to the copy
			throw O;
		} catch( const verbose_object &E ) {
			CPPUNIT_ASSERT_MESSAGE("Thrown reference should have !defaultConstructed", !E.defaultConstructed);
			CPPUNIT_ASSERT_MESSAGE("Thrown reference should have copyConstructed", E.copyConstructed);
			CPPUNIT_ASSERT_MESSAGE("Thrown reference should have !moveConstructed", !E.moveConstructed);
			CPPUNIT_ASSERT_MESSAGE("Thrown reference shouldn't match object",
				&O != &E);
		}
	}
	void testMovedException() {
		try {
			throw std::move(verbose_object());
		} catch( const verbose_object &E ) {
			CPPUNIT_ASSERT_MESSAGE("Thrown reference should have !defaultConstructed", !E.defaultConstructed);
			CPPUNIT_ASSERT_MESSAGE("Thrown reference should have !copyConstructed", !E.copyConstructed);
			CPPUNIT_ASSERT_MESSAGE("Thrown reference should have moveConstructed", E.moveConstructed);
		}
	}

	// --- Auto-generate suite() convenience function
	CPPUNIT_TEST_SUITE(ExceptionTest);
	CPPUNIT_TEST(testDefaultConstructor);
	CPPUNIT_TEST(testDestructor);
	CPPUNIT_TEST(testCopyConstructor);
	CPPUNIT_TEST(testMoveConstructor);
	CPPUNIT_TEST(testCopyAssignment);
	CPPUNIT_TEST(testMoveAssignment);
	CPPUNIT_TEST(testDefaultedException);
	CPPUNIT_TEST(testCopiedException);
	CPPUNIT_TEST(testMovedException);
	CPPUNIT_TEST_SUITE_END();
};
// Add result of AxiomsTest::suite() to test registry
CPPUNIT_TEST_SUITE_REGISTRATION( ExceptionTest );


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
