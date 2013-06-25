#include <iostream>
#include <condition_variable>
#include <mutex>
#include <queue>
#include <algorithm>
#include <memory>
using namespace std;


template<typename T>
class TLockingQueue
{
  public:
	TLockingQueue() = default;
	~TLockingQueue() = default;
	TLockingQueue(const TLockingQueue& O ) :
		TLockingQueue() {
		// We're going to mess with the source, so lock it, we needn't
		// worry about our lock, no one else can have us yet because
		// we're constructing.
		unique_lock<mutex> lock(O.mQueueLock);
		// Copy constructor does a deep-copy
		mQueue = O.mQueue;
		// Note: if the underlying type, T, cannot be copied, then it is
		// that type's responsibility to delete its copy-constructor
	}
	TLockingQueue(TLockingQueue&& O) :
		TLockingQueue() {
		swap(O);
	}
	// Copy/move assignment operator in one
	TLockingQueue& operator=(TLockingQueue O) {
		swap(O);
		return *this;
	}

	void swap(TLockingQueue &O) {
		using std::swap;
		using std::lock;
		// Scoped locks, left unlocked
		unique_lock<mutex> lockOurs(mQueueLock, defer_lock);
		unique_lock<mutex> lockTheirs(O.mQueueLock, defer_lock);
		// Multiple locks should be an unordered lock, adopt them
		lock(lockOurs, lockTheirs);

		// No need to swap the locks, they're both locked

		swap(mQueue, O.mQueue);

		// Swapping the wait conditions would be dangerous, what if
		// someone is already listening?  Better to just wake them
		// because something has changed
		if(!mQueue.empty())
			mQueueEvent.notify_one();
		if(!O.mQueue.empty())
			O.mQueueEvent.notify_one();
	}

	// --------------------

	void push(T&& O) {
		unique_lock<mutex> lock(mQueueLock);
		// Force use of move-push
		mQueue.push(move(O));
		mQueueEvent.notify_one();
	}

	void push(const T &O) {
		unique_lock<mutex> lock(mQueueLock);
		mQueue.push(O);
		mQueueEvent.notify_one();
	}

	T pop() {
		unique_lock<mutex> lock(mQueueLock);
		if( mQueue.empty() ) {
			// If the queue is empty, sleep until it isn't
			mQueueEvent.wait(lock);
		}
		// Events can wake up even if queue is empty
		if( mQueue.empty() )
			throw runtime_error("Wakeup with empty queue");

		// We can safely use move since we're about to pop() If there is
		// no move constructor for T, this will just copy construct.
		T ret {move(mQueue.front())};
		mQueue.pop();
		// Rely on T having move semantics if it wants speed
		return ret;
	}

	bool empty() const {
		unique_lock<mutex> lock(mQueueLock);
		return mQueue.empty();
	}

	size_t size() const {
		unique_lock<mutex> lock(mQueueLock);
		return mQueue.size();
	}

	void wake() {
		mQueueEvent.notify_one();
	}

  protected:
	mutable mutex mQueueLock;
	queue<T> mQueue;
	condition_variable mQueueEvent;
};

using TPointerQueue = TLockingQueue<unique_ptr<int>>;


int main()
{
	TPointerQueue q;

	q.push( unique_ptr<int>( new int(0) ) );
	q.push( unique_ptr<int>( new int(1) ) );
	q.push( unique_ptr<int>( new int(2) ) );
	q.push( unique_ptr<int>( new int(3) ) );

	cerr << "Creating queue" << endl;
	cerr << "q.size() = " << q.size() << endl;

	cerr << "Moving constructing queue from q to q2" << endl;
	// Can't copy unique_ptr, so can't copy a queue of unique_ptr; but
	// we can move it, this should leave q empty
	TPointerQueue q2 {std::move(q)};

	cerr << "q.size() = " << q.size() << endl;
	cerr << "q2.size() = " << q2.size() << endl;

	// pop
	unique_ptr<int> p = q2.pop();

	return 0;
}
