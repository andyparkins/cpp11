// http://solarianprogrammer.com/2012/10/14/cpp-11-timing-code-performance/

#include <iostream>
#include <chrono>
#include <typeinfo>
#include <cxxabi.h>

using namespace std;

#define HAVE_CXA_DEMANGLE 1
#ifdef HAVE_CXA_DEMANGLE
const string demangle(const char* name)
{
	int status = -4;

	char* res = abi::__cxa_demangle(name, NULL, NULL, &status);

	const char* const demangled_name = (status==0)?res:name;

	string ret_val(demangled_name);

	free(res);

	return ret_val;
}
#else
const char* demangle(const char* name)
{
	return name;
}
#endif


int main()
{
	auto start = chrono::steady_clock::now();

	cout << "system_clock tick "
		<< chrono::system_clock::period::num
		<< " / " << chrono::system_clock::period::den << endl;
	cout << "steady = " << boolalpha << chrono::system_clock::is_steady << endl << endl;

	cout << "high_resolution_clock tick "
		<< chrono::high_resolution_clock::period::num
		<< " / " << chrono::high_resolution_clock::period::den << endl;
	cout << "steady = " << boolalpha << chrono::high_resolution_clock::is_steady << endl << endl;

	cout << "steady_clock tick "
		<< chrono::steady_clock::period::num
		<< " / " << chrono::steady_clock::period::den << endl;
	cout << "steady = " << boolalpha << chrono::steady_clock::is_steady << endl << endl;

	auto end = chrono::steady_clock::now();

	// Store the time difference between start and end
	auto diff = end - start;
	auto diff_us = chrono::duration_cast<chrono::microseconds>(diff);

	cerr << "start.type    = " << demangle(typeid(start).name()) << endl;
	cerr << "diff.type     = " << demangle(typeid(diff).name()) << endl;
	cerr << "diff_us.type  = " << demangle(typeid(diff_us).name()) << endl;

	cout << "t_ms (double)   " << chrono::duration<double, milli>(diff).count() << " ms" << endl;
	cout << "t_ns (double)   " << chrono::duration<double, nano>(diff).count() << " ns" << endl;
	cout << "t_us (double)   " << chrono::duration<double, micro>(diff).count() << " us" << endl;
	cout << "t_us (trunc)    " << diff_us.count() << endl;

	return 0;
}

