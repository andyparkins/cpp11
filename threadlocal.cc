#include <iostream>
#include <thread>
using namespace std;

constexpr int INIT = 10;

#ifndef __clang__
// Last I tried this wasn't supported by clang 3.2, but does work with
// gcc 4.8
thread_local int x = INIT;
#else
#	warning "thread_local not known to work yet on clang"
int x = INIT;
#endif


void incrementX()
{
	cerr << "OFFTHREAD  x = " << x << endl;
	// On thread creation, thread_local storage should be copied from
	// the creating thread
	if( x != INIT )
		throw runtime_error("thread_local storage should init from creator");

	x++;
	cerr << "OFFTHREAD  x = " << x << endl;

	if( x != INIT+1 )
		throw runtime_error("thread_local storage should have changed");
}


int main()
{
	cerr << "MAINTHREAD x = " << x << endl;
	thread t1(incrementX);
	t1.join();
	cerr << "MAINTHREAD x = " << x << endl;

	if( x != INIT )
		throw runtime_error("thread_local storage shouldn't have changed");

	return 0;
}
