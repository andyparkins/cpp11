// ----------------------------------------------------------------------------
// Project: lua
/// @file   lua.cc
/// @author Andy Parkins
//
// Version Control
//    $Author$
//      $Date$
//        $Id: 460bbb396cb2ed4325cf653d88a0e1ec8feb2454 $
//
// Legal
//    Copyright 2013  FussyLogic Ltd
//
// ----------------------------------------------------------------------------

// Module include
//#include "template.h"

// -------------- Includes
// --- C
// --- C++
// --- OS
// --- Libraries
#include <lua5.2/lua.hpp>
// --- Project libs


// -------------- Namespace


// -------------- Macros


// -------------- Module Globals


// -------------- World Globals (need "extern"s in header)


// -------------- Class member static initialisers


// -------------- Class member definitions


// -------------- Function definitions


// -------------- Unit test
#ifdef UNITTEST
#include <cppunit/extensions/HelperMacros.h>

#include <string>

using namespace std;

//
// Class:	LuaTest
// Description:
//  A TestFixture groups multiple tests in a common environment; we
//  assume that the common environment is costly or inconvenient to
//  manufacture, so we do multiple tests with that one environment.
//
class LuaTest : public CppUnit::TestFixture
{
  protected:
	lua_State *L;

	typedef int (*lua_CFunction)(lua_State *L);
	static int x;
	static int customFunction(lua_State *L) {
		// Number of elements on the parameter stack
		int n = lua_gettop(L);
		if( n == 0 )
			return 0;

		for(int i = 0; i < n; i++) {
			if( !lua_isnumber(L, i+1) ) {
				lua_pushstring(L, "incorrect arguments");
				lua_error(L);
			}
			x += lua_tointeger(L, i+1);
		}

		// return x
		lua_pushinteger(L, x);
		return 1;
	}

  public:
	// Construct test environment
	void setUp(void) {
		L = luaL_newstate();
		// Create a Lua virtual machine
		CPPUNIT_ASSERT(L != nullptr);
		x = 0;
	}
	// Destruct test environment
	void tearDown(void) {
		lua_close(L);
	}

	void testLuaOpen() {

		// Make all the lua standard libs available to the virtual machine
		luaL_openlibs(L);

		// Test it (success returns false)
		CPPUNIT_ASSERT_EQUAL(true, !luaL_dostring(L, "return 50*30"));
		CPPUNIT_ASSERT_EQUAL(1, lua_gettop(L));
		int y = lua_tonumber(L, -1);
		CPPUNIT_ASSERT_EQUAL(1500, y);

		// Clear the stack
		lua_settop(L, 0);
	}

	void testCFromLua() {
		int y;

		CPPUNIT_ASSERT_EQUAL(0, x);

		// Register our function
		lua_register(L, "customFunction", customFunction);

		// Run the custom function with no arguments, should just return
		// x
		x = 0x67384562;
		CPPUNIT_ASSERT_EQUAL(true, !luaL_dostring(L, "return customFunction()"));
		CPPUNIT_ASSERT_EQUAL(0x67384562, x);
		CPPUNIT_ASSERT_EQUAL(0, lua_gettop(L));

		CPPUNIT_ASSERT_EQUAL(true, !luaL_dostring(L, "return customFunction(1)"));
		CPPUNIT_ASSERT_EQUAL(0x67384562 + 1, x);
		CPPUNIT_ASSERT_EQUAL(1, lua_gettop(L));
		y = lua_tointeger(L, -1);
		CPPUNIT_ASSERT_EQUAL(0x67384562 + 1, y);
	}

	void testLuaFromC() {
		// Define the lua callback in the state; note that this doesn't
		// call the function, it creates the reference to it in the
		// global table
		CPPUNIT_ASSERT_EQUAL(true, !luaL_dostring(L, R"***(
			function onEvent(a)
				return a+10
			end
		)***"));

		x = 0x7673f562;

		// What function do we want to call?  Find it and push it onto
		// the stack
		lua_getglobal(L, "onEvent");
		// Expect that the pushed value is not nil
		CPPUNIT_ASSERT_EQUAL(1, lua_gettop(L));
		int n = lua_isnil(L, -1);
		CPPUNIT_ASSERT_EQUAL(0, n);

		// Then push the arguments
		lua_pushnumber(L, x);
		// Call it, telling Lua it has one argument, one result
		lua_call(L, 1, 1);
		// Fetch the result
		int y = lua_tointeger(L, -1);
		// Restore the stack
		lua_pop(L, 1);

		CPPUNIT_ASSERT_EQUAL(x+10, y);
	}

	void testNotLuaFromC() {
		lua_getglobal(L, "NON_EXISTANT_FUNCTION");
		// Expect that the pushed value is nil
		CPPUNIT_ASSERT_EQUAL(1, lua_gettop(L));
		int n = lua_isnil(L, -1);
		CPPUNIT_ASSERT_EQUAL(1, n);

		// Then push the arguments
		lua_pushnumber(L, x);
		// Call it, telling Lua it has one argument, one result
		CPPUNIT_ASSERT_EQUAL(LUA_ERRRUN, lua_pcall(L, 1, 1, 0));
		// The protected call should fail when the call is not defined,
		// we should get an error message on the stack
		CPPUNIT_ASSERT_EQUAL(1, lua_gettop(L));
		// And the error should be... 
		CPPUNIT_ASSERT_EQUAL(string("attempt to call a nil value"),
				string(lua_tostring(L, -1)));
	}

	// -----------------------

	// HelperMacros to manufacture `static CppUnit::Test *suite()`
	CPPUNIT_TEST_SUITE(LuaTest);
	CPPUNIT_TEST(testLuaOpen);
	CPPUNIT_TEST(testCFromLua);
	CPPUNIT_TEST(testNotLuaFromC);
	CPPUNIT_TEST(testLuaFromC);
	CPPUNIT_TEST_SUITE_END();
};
// HelperMacro to auto-call LuaTest::suite() and add it to a
// single registry
CPPUNIT_TEST_SUITE_REGISTRATION(LuaTest);

int LuaTest::x = 0;


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
	return runner.run() ? 0 : 1;
}
#endif
#endif

