// -------------- Includes
// --- C
// --- C++
#include <bitset>
#include <iostream>
// --- OS
// --- Project libs


// -------------- Namespace
using namespace std;


// -------------- Macros


// -------------- Module Globals


// -------------- World Globals (need "extern"s in header)


// -------------- Class member static initialisers


// -------------- Enumerations


// -------------- Class member definitions
template<unsigned int N>
class bitset_extra : public bitset<N>
{
  public:
	// Add our own conversion from any bitset
	template<unsigned int S>
	bitset_extra(const bitset<S> &O) :
		bitset<N>()
	{
		for(unsigned int i = 0; i < N && i < S; i++ ) {
			(*this)[i] = O[i];
		}
	}
};


// -------------- Function definitions


// -------------- Unit test
#ifdef UNITTEST
#include <iostream>
using namespace std;


#include <cppunit/extensions/HelperMacros.h>
//
// Class:	BitsetTest
// Description:
//  A TestFixture groups multiple tests in a common environment; we
//  assume that the common environment is costly or inconvenient to
//  manufacture, so we do multiple tests with that one environment.
//
class BitsetTest : public CppUnit::TestFixture
{
  public:
	// Construct test environment
	void setUp(void) {}
	// Destruct test environment
	void tearDown(void) {}

	void testBitsetSize() {
		bitset<1> bs1;
		CPPUNIT_ASSERT_EQUAL(4U, sizeof(bs1) );
		bitset<2> bs2;
		CPPUNIT_ASSERT_EQUAL(4U, sizeof(bs2) );
		bitset<7> bs7;
		CPPUNIT_ASSERT_EQUAL(4U, sizeof(bs7) );
		bitset<8> bs8;
		CPPUNIT_ASSERT_EQUAL(4U, sizeof(bs8) );
		bitset<15> bs15;
		CPPUNIT_ASSERT_EQUAL(4U, sizeof(bs15) );
		bitset<16> bs16;
		CPPUNIT_ASSERT_EQUAL(4U, sizeof(bs16) );
		bitset<23> bs23;
		CPPUNIT_ASSERT_EQUAL(4U, sizeof(bs23) );
		bitset<24> bs24;
		CPPUNIT_ASSERT_EQUAL(4U, sizeof(bs24) );
		bitset<31> bs31;
		CPPUNIT_ASSERT_EQUAL(4U, sizeof(bs31) );
		bitset<32> bs32;
		CPPUNIT_ASSERT_EQUAL(4U, sizeof(bs32) );
		bitset<33> bs33;
		CPPUNIT_ASSERT_EQUAL(8U, sizeof(bs33) );
		bitset<63> bs63;
		CPPUNIT_ASSERT_EQUAL(8U, sizeof(bs63) );
		bitset<64> bs64;
		CPPUNIT_ASSERT_EQUAL(8U, sizeof(bs64) );
		bitset<65> bs65;
		CPPUNIT_ASSERT_EQUAL(12U, sizeof(bs65) );
	}

	void testBitsetOps() {
		unsigned long a = 0xad6274;
		unsigned long b = 0x9736d5af;
		bitset<32> bs1( a );
		bitset<32> bs2( b );

		CPPUNIT_ASSERT_EQUAL(a ^ b, (bs1 ^ bs2).to_ulong());
		CPPUNIT_ASSERT_EQUAL(a | b, (bs1 | bs2).to_ulong());
		CPPUNIT_ASSERT_EQUAL(a & b, (bs1 & bs2).to_ulong());
		CPPUNIT_ASSERT_EQUAL(~a, (~bs1).to_ulong());
		CPPUNIT_ASSERT_EQUAL(~b, (~bs2).to_ulong());

		bitset<32> bs3;
		CPPUNIT_ASSERT_EQUAL(0UL, bs3.to_ulong());
		bs3.set(0);
		CPPUNIT_ASSERT_EQUAL(1UL, bs3.to_ulong());
		bs3.reset(0);
		CPPUNIT_ASSERT_EQUAL(0UL, bs3.to_ulong());
		bs3.flip(0);
		bs3 <<= 1;
		CPPUNIT_ASSERT_EQUAL(2UL, bs3.to_ulong());
		bs3.flip(0);
		bs3 <<= 1;
		CPPUNIT_ASSERT_EQUAL(6UL, bs3.to_ulong());

		// Shame, bitset<> has no way to construct from another bitset
		unsigned long long c = (((unsigned long long)(a) << 32) | b);
		bitset_extra<64> bs4(bs1);
		bitset_extra<64> bs5(bs2);
		CPPUNIT_ASSERT_EQUAL(c, ((bs4 << 32) | bs5).to_ullong());
	}

	// -----------------------

	// HelperMacros to manufacture `static CppUnit::Test *suite()`
	CPPUNIT_TEST_SUITE(BitsetTest);
	CPPUNIT_TEST(testBitsetSize);
	CPPUNIT_TEST(testBitsetOps);
	CPPUNIT_TEST_SUITE_END();
};
// HelperMacro to auto-call BitsetTest::suite() and add it to a
// single registry
CPPUNIT_TEST_SUITE_REGISTRATION(BitsetTest);


// -------------- main()
#ifndef UNITTESTALL
// --- cppunit run includes
#include <cppunit/ui/text/TestRunner.h>
#include <cppunit/TextOutputter.h>
// ---
#include <iostream>
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
#endif
#endif


