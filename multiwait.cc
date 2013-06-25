#include <iostream>
#include <condition_variable>
#include <mutex>
#include <thread>
#include <chrono>
#include <set>
#include <map>
#include <memory>

using namespace std;

class TNotifier;

class TEventWaiter
{
  public:
	TEventWaiter() = default;
	TEventWaiter(initializer_list<TNotifier*> &il);
	~TEventWaiter();

	// Sleeping/waking
	bool sleep();
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
	TNotifier();

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
	void wake() const {
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



TEventWaiter::TEventWaiter(initializer_list<TNotifier*> &il) {
		for( auto e : il )
			watch(e);
}

TEventWaiter::~TEventWaiter() {
	unique_lock<mutex> L(mEventsLock);

	// Can't use unwatch(), it will wreck the iterators
	for( auto e : mEvents )
		e.first->unsubscribe(this);

	mEvents.clear();
}

bool TEventWaiter::sleep() {
	unique_lock<mutex> L(mEventsLock);
	if( mEvents.empty() )
		return false;
	// Check for anything already pending while we weren't asleep
	for( auto e : mEvents ) {
		if( e.second )
			return true;
		e.second = e.first->notified();
		if( e.second )
			return true;
	}
	// No events pending, and we've got the lock held so no one
	// could have changed the event while we were busy.  We can
	// sleep and unlock atomically
	mEvent.wait(mEventsLock);
	// The wake() call has already set pending flags so we don't
	// need to do anything other than check them again for the
	// return flag
	for( auto e : mEvents ) {
		if( e.second )
			return true;
	}
	return false;
}

void TEventWaiter::wake(TNotifier *N) {
	unique_lock<mutex> L(mEventsLock);
	auto it = mEvents.find(N);
	if( it == mEvents.end() )
		return;
	// Set it pending
	it->second = true;
	// Wake -- there can only be one listener, so notify_one() or
	// notify_all() doesn't matter
	mEvent.notify_one();
}

void TEventWaiter::watch( TNotifier *N ) {
	unique_lock<mutex> L(mEventsLock);
	O->subscribe(this);
}

void TEventWaiter::unwatch( TNotifier *N ) {
	unique_lock<mutex> L(mEventsLock);
	auto it = mEvents.find(N);
	if( it == mEvents.end() )
		return;
	mEvents.erase(it);
	O->unsubscribe(this);
}

// -----------------------

void delayThenEvent(TNotifier *E, const chrono::duration &dura)
{
	this_thread::sleep_for( dura );
	E->wakeOne();
}

int main()
{
//	TNotifier E1, E2;
//	TEventWaiter EW {E1, E2};
//
//	thread t1(delayThenEvent(&E1), chrono::milliseconds( 1500 ));
//	thread t2(delayThenEvent(&E2), chrono::milliseconds( 1000 ));
//
//	EW.wait();

	return 0;
}
