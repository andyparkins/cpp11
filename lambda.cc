#include <iostream>
#include <vector>
#include <iterator>
#include <algorithm>
using namespace std;


class TRow {
  public:
	TRow(int i) : x(i) {}

	int x;
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

class LambdaTest : public CppUnit::TestFixture
{
	vector<TRow> Rows;
  public:
	// --- Prepare
	void setUp() {
		// initializer_list of {} initializers
		Rows = {{0}, {1}, {2}, {3}, {4}, {5}, {6}, {7}, {8}, {9}};
	}
	void tearDown() { }

	// --- Tests
	void testSetup() {
		CPPUNIT_ASSERT_EQUAL(0, Rows[0].x);
		CPPUNIT_ASSERT_EQUAL(1, Rows[1].x);
		CPPUNIT_ASSERT_EQUAL(2, Rows[2].x);
		CPPUNIT_ASSERT_EQUAL(3, Rows[3].x);
		CPPUNIT_ASSERT_EQUAL(4, Rows[4].x);
		CPPUNIT_ASSERT_EQUAL(5, Rows[5].x);
		CPPUNIT_ASSERT_EQUAL(6, Rows[6].x);
		CPPUNIT_ASSERT_EQUAL(7, Rows[7].x);
		CPPUNIT_ASSERT_EQUAL(8, Rows[8].x);
		CPPUNIT_ASSERT_EQUAL(9, Rows[9].x);
	}

	static bool predicate1(const TRow &O) {
		return O.x < 5;
	}
	static bool predicate2(const TRow &O) {
		return O.x > 5;
	}

	void testManualSearch() {
		vector<TRow> Predicate1;
		for( const auto &row : Rows ) {
			if( predicate1(row) ) {
				Predicate1.push_back(row);
			}
		}
		vector<TRow> Predicate2;
		for( const auto &row : Rows ) {
			if( predicate2(row) ) {
				Predicate2.push_back(row);
			}
		}
		CPPUNIT_ASSERT_EQUAL(5U, Predicate1.size());
		CPPUNIT_ASSERT_EQUAL(0, Predicate1[0].x);
		CPPUNIT_ASSERT_EQUAL(1, Predicate1[1].x);
		CPPUNIT_ASSERT_EQUAL(2, Predicate1[2].x);
		CPPUNIT_ASSERT_EQUAL(3, Predicate1[3].x);
		CPPUNIT_ASSERT_EQUAL(4, Predicate1[4].x);
		CPPUNIT_ASSERT_EQUAL(4U, Predicate2.size());
		CPPUNIT_ASSERT_EQUAL(6, Predicate2[0].x);
		CPPUNIT_ASSERT_EQUAL(7, Predicate2[1].x);
		CPPUNIT_ASSERT_EQUAL(8, Predicate2[2].x);
		CPPUNIT_ASSERT_EQUAL(9, Predicate2[3].x);
	}

	void testAlgorithmSearch() {
		vector<TRow> Predicate1;
		copy_if(Rows.cbegin(), Rows.cend(), back_inserter(Predicate1), predicate1);
		vector<TRow> Predicate2;
		copy_if(Rows.cbegin(), Rows.cend(), back_inserter(Predicate2), predicate2);
		CPPUNIT_ASSERT_EQUAL(5U, Predicate1.size());
		CPPUNIT_ASSERT_EQUAL(0, Predicate1[0].x);
		CPPUNIT_ASSERT_EQUAL(1, Predicate1[1].x);
		CPPUNIT_ASSERT_EQUAL(2, Predicate1[2].x);
		CPPUNIT_ASSERT_EQUAL(3, Predicate1[3].x);
		CPPUNIT_ASSERT_EQUAL(4, Predicate1[4].x);
		CPPUNIT_ASSERT_EQUAL(4U, Predicate2.size());
		CPPUNIT_ASSERT_EQUAL(6, Predicate2[0].x);
		CPPUNIT_ASSERT_EQUAL(7, Predicate2[1].x);
		CPPUNIT_ASSERT_EQUAL(8, Predicate2[2].x);
		CPPUNIT_ASSERT_EQUAL(9, Predicate2[3].x);
	}

	void testLambdaSearch() {
		vector<TRow> Predicate1;
		copy_if(Rows.cbegin(), Rows.cend(), back_inserter(Predicate1),
				[](const TRow& O){ return O.x < 5;} );
		vector<TRow> Predicate2;
		copy_if(Rows.cbegin(), Rows.cend(), back_inserter(Predicate2),
				[](const TRow& O){ return O.x > 5;} );

		CPPUNIT_ASSERT_EQUAL(5U, Predicate1.size());
		CPPUNIT_ASSERT_EQUAL(0, Predicate1[0].x);
		CPPUNIT_ASSERT_EQUAL(1, Predicate1[1].x);
		CPPUNIT_ASSERT_EQUAL(2, Predicate1[2].x);
		CPPUNIT_ASSERT_EQUAL(3, Predicate1[3].x);
		CPPUNIT_ASSERT_EQUAL(4, Predicate1[4].x);
		CPPUNIT_ASSERT_EQUAL(4U, Predicate2.size());
		CPPUNIT_ASSERT_EQUAL(6, Predicate2[0].x);
		CPPUNIT_ASSERT_EQUAL(7, Predicate2[1].x);
		CPPUNIT_ASSERT_EQUAL(8, Predicate2[2].x);
		CPPUNIT_ASSERT_EQUAL(9, Predicate2[3].x);
	}

	// --- Auto-generate suite() convenience function
	CPPUNIT_TEST_SUITE(LambdaTest);
	CPPUNIT_TEST(testSetup);
	CPPUNIT_TEST(testManualSearch);
	CPPUNIT_TEST(testAlgorithmSearch);
	CPPUNIT_TEST(testLambdaSearch);
	CPPUNIT_TEST_SUITE_END();
};
// Add result of AxiomsTest::suite() to test registry
CPPUNIT_TEST_SUITE_REGISTRATION( LambdaTest );


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
