#include <iostream>
#include <memory>
using namespace std;


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

class PointerTest : public CppUnit::TestFixture
{
  public:
	// --- Prepare
	void setUp() { }
	void tearDown() { }

	// --- Tests
	void testUniquePointer() {
		unique_ptr<int> p1 { new int };
		unique_ptr<int> p2;
		int *vp1 = p1.get();

		// p1 should exist
		CPPUNIT_ASSERT(p1.get() != nullptr);
		// p2 shouldn't exist
		CPPUNIT_ASSERT_EQUAL((int*)(nullptr), p2.get());
		// p1 shouldn't equal p2
		CPPUNIT_ASSERT(p1.get() != p2.get());

		// This is illegal, can't copy a unique_ptr
		//	p2 = p1;
		// We can, however, move one
		p2 = std::move(p1);

		// p2 should no longer be null
		CPPUNIT_ASSERT(p2.get() != nullptr);
		// p1 now should be null
		CPPUNIT_ASSERT_EQUAL((int*)(nullptr), p1.get());
		// p1 should still not equal p2
		CPPUNIT_ASSERT(p1.get() != p2.get());
		// The new p2 should equal the old p1
		CPPUNIT_ASSERT_EQUAL(vp1, p2.get());
	}

	void testSharedPointer() {
		shared_ptr<int> p1 { new int };
		shared_ptr<int> p2;
		int *vp1 = p1.get();

		// p1 should exist
		CPPUNIT_ASSERT(p1.get() != nullptr);
		// p2 shouldn't exist
		CPPUNIT_ASSERT_EQUAL((int*)(nullptr), p2.get());
		// p1 shouldn't equal p2
		CPPUNIT_ASSERT(p1.get() != p2.get());
		// p1 should be unique
		CPPUNIT_ASSERT(p1.unique());

		// Shared
		p2 = p1;

		// p2 should no longer be null
		CPPUNIT_ASSERT(p2.get() != nullptr);
		// The new p1 should equal the old p1
		CPPUNIT_ASSERT_EQUAL(vp1, p1.get());
		// The new p2 should equal the old p1
		CPPUNIT_ASSERT_EQUAL(p1.get(), p2.get());
		// no unique
		CPPUNIT_ASSERT(!p1.unique());
		CPPUNIT_ASSERT(!p2.unique());
		// Count
		CPPUNIT_ASSERT_EQUAL((long)2, p1.use_count());
		CPPUNIT_ASSERT_EQUAL((long)2, p2.use_count());

		int *vp2 = p2.get();
		p1.reset();
		// The new p1 should equal null
		CPPUNIT_ASSERT_EQUAL((int*)(nullptr), p1.get());
		// The new p2 should equal the old p2
		CPPUNIT_ASSERT_EQUAL(vp2, p2.get());
		// p2 should be unique
		CPPUNIT_ASSERT(p2.unique());


		// Note that shared_ptr doesn't stop you from being stupid:
		//  int *pi = new int;
		//  shared_ptr<int> spi {pi};
		//  delete pi;
		// CRASH!!!!
	}

	// --- Auto-generate suite() convenience function
	CPPUNIT_TEST_SUITE(PointerTest);
	CPPUNIT_TEST(testUniquePointer);
	CPPUNIT_TEST(testSharedPointer);
	CPPUNIT_TEST_SUITE_END();
};
// Add result of AxiomsTest::suite() to test registry
CPPUNIT_TEST_SUITE_REGISTRATION(PointerTest);


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
