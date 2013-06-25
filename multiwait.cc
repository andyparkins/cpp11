#include <iostream>
#include <condition_variable>
#include <mutex>
#include <thread>
#include <chrono>
#include <set>
#include <map>
#include <memory>
#include <algorithm>

using namespace std;

class TNotifier;

class TEventWaiter
{
  public:
	TEventWaiter() = default;
	TEventWaiter(initializer_list<TNotifier&> &il);
	~TEventWaiter();
	// Refuse to copy, we only have one pending flag
	TEventWaiter(const TEventWaiter &) = delete;
	TEventWaiter& operator=(const TEventWaiter &) = delete;
	// Move possible but hard, so disallow
	TEventWaiter(TEventWaiter &&) = delete;
	TEventWaiter& operator=(TEventWaiter &&) = delete;

	// Sleeping/waking
	vector<const TNotifier *> wait();
	void wake(TNotifier *N);

	// Establish listeners
	void watch( TNotifier *N );
	void unwatch( TNotifier *N );

  protected:
	condition_variable mEvent;
	mutex mEventsLock;
	map<TNotifier *, bool> mEvents;
};

class TNotifier
{
  public:
	TNotifier() = default;

	// Subscription
	void subscribe( TEventWaiter *EW ) {
		unique_lock<mutex> L(mSubscriberLock);
		mSubscribers.insert(EW);
	};
	void unsubscribe( TEventWaiter *EW ) {
		unique_lock<mutex> L(mSubscriberLock);
		auto it = mSubscribers.find(EW);
		if( it == mSubscribers.end() )
			return;
		mSubscribers.erase(it);
	}

	// Wakes
	void wake() {
		unique_lock<mutex> L(mSubscriberLock);
		for( TEventWaiter *EW : mSubscribers ) {
			EW->wake(this);
		}
	}

	// Child classes might have long-lived events
	virtual bool notified() const { return false; }

  protected:
	mutable mutex mSubscriberLock;
	set<TEventWaiter *> mSubscribers;
};



TEventWaiter::TEventWaiter(initializer_list<TNotifier&> &il)
{
}

TEventWaiter::~TEventWaiter()
{
	unique_lock<mutex> L(mEventsLock);

	// Can't use unwatch(), it will wreck the iterator used in this loop
	for( auto e : mEvents )
		e.first->unsubscribe(this);

	mEvents.clear();
}

vector<const TNotifier *> TEventWaiter::wait()
{
	unique_lock<mutex> L(mEventsLock);
	// Move-semantics lets us easily return the wakers
	vector<const TNotifier*> wakers;

	if( mEvents.empty() )
		return wakers;

	// Check for anything already pending while we weren't asleep
	for( auto e : mEvents ) {
		if( e.second ) {
			e.second = false;
			wakers.push_back(e.first);
		}
	}
	if( !wakers.empty() )
		return wakers;

	// Check for would-have-signalled
	for( auto e : mEvents ) {
		if( e.first->notified() )
			wakers.push_back(e.first);
	}
	if( !wakers.empty() )
		return wakers;

	// No events pending, and we've got the lock held so no one
	// could have changed the event while we were busy.  We can
	// sleep and unlock atomically
	mEvent.wait(L);

	// Our wake member will have set the appropriate pending flags, so
	// we just build the return structure
	for( auto e : mEvents ) {
		if( e.second ) {
			e.second = false;
			wakers.push_back(e.first);
		}
	}

	return wakers;
}

void TEventWaiter::wake(TNotifier *N)
{
	unique_lock<mutex> L(mEventsLock);
	auto it = mEvents.find(N);
	if( it == mEvents.end() )
		return;
	// Set it pending so wait() can tell who woke
	it->second = true;
	// Wake -- there can only be one listener, so notify_one() or
	// notify_all() doesn't matter
	mEvent.notify_one();
}

void TEventWaiter::watch( TNotifier *N )
{
	unique_lock<mutex> L(mEventsLock);
	mEvents[N] = false;
	N->subscribe(this);
}

void TEventWaiter::unwatch( TNotifier *N )
{
	unique_lock<mutex> L(mEventsLock);
	auto it = mEvents.find(N);
	if( it == mEvents.end() )
		return;
	mEvents.erase(it);
	N->unsubscribe(this);
}


// -----------------------

void delayThenEvent(TNotifier *E, const chrono::milliseconds &dura)
{
	this_thread::sleep_for( dura );
	cerr << this_thread::get_id() << " waking after " << dura.count() << "ms" << endl;
	E->wake();
}

int main()
{
	TNotifier E1, E2;
	TEventWaiter EW;

	// After watch, we cannot miss an event, we therefore always start
	// watching before we initiate the action
	EW.watch(&E1);
	EW.watch(&E2);

	// Make two basic threads to do nothing but delay then signal
	thread t1(bind(delayThenEvent, &E1, chrono::milliseconds( 5500 )));
	thread t2(bind(delayThenEvent, &E2, chrono::milliseconds( 5000 )));

	cerr << "Waiting for event" << endl;
	auto start = chrono::steady_clock::now();
	EW.wait();
	chrono::milliseconds took =
		chrono::duration_cast<chrono::milliseconds>(
				chrono::steady_clock::now() - start);
	cerr << "Event took " << took.count() << "ms" << endl;

	cerr << "Waiting for threads to complete" << endl;
	t1.join();
	t2.join();

	return 0;
}
