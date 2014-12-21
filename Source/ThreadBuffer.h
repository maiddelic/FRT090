#pragma once

/**
 * Thread safe ring buffer. This is a template class which means
 * that it can handle any type of object, here stated as T.
 * The buffer is not currently able to handle
 * unmanaged pointers, and will cause memory leaks if a pointer is overwritten,
 * as long as it is not simultaneously known and deleted elsewhere.
 *
 */
template<class T> ref class ThreadBuffer
{
public:
	/**
	 * Create empty buffer with specified size bufSize. 
	 */
	ThreadBuffer(size_t bufSize) {
		 buf = new T[bufSize];
		 lock = gcnew System::Object();

		 this->bufSize = bufSize;
		 putIndex = 0;
		 getIndex = 0;
		 numEntries = 0;

		 
	}
	/**
	*Destructor. Releases allocated memory
	*/
	~ThreadBuffer(void) {
		delete[] buf;
		delete lock;
	}

	
	/**
	 *	Place item in buffer. If the buffer is full, the oldest item is 
	 *  overwritten. No attempt to delete overwritten items is made, making
	 *  storage of unmanaged unsafe.
	 */
	void put(T elem) {

		System::Threading::Monitor::Enter(lock);

		try {
			buf[putIndex] = elem;
			
			if (putIndex == getIndex) {
				if (numEntries == 0) { //buffer was empty
					getIndex = putIndex;

				} else {	//last element was overwritten, increment getIndex
					getIndex = getIndex == bufSize - 1 ? 0 : getIndex + 1;
				}
			}
			
			//increment putIndex. If the next place to put the object is at the end of the buffer it will be placed on 
			//index 0. Else just increment the index where to put the object.
			putIndex = putIndex == bufSize - 1 ? 0 : putIndex + 1;
			
			if (numEntries < bufSize) {
				numEntries++;
			}

		} finally {
			//Release monitor even if execution fails somehow
			System::Threading::Monitor::PulseAll(lock);
			System::Threading::Monitor::Exit(lock);
		}
	}


	/**
	 * Retrieve element from buffer. Blocking.
	 */
	T get() {
		T retVal;

		System::Threading::Monitor::Enter(lock);
		try {
			while (numEntries == 0) { // while no objects in buffer, wait.
				System::Threading::Monitor::Wait(lock);
			}

			retVal = buf[getIndex];
			getIndex = getIndex == bufSize - 1 ? 0 : getIndex + 1;
			numEntries--;

		} finally {
			System::Threading::Monitor::PulseAll(lock);
			System::Threading::Monitor::Exit(lock);
		}

		return retVal;
	}

	/**
	 *  Remove all elements from buffer. If the buffer contains unmanaged
	 *  pointers, the memory is lost unless it is deallocated elsewhere.
	 */
	void flush() {
		System::Threading::Monitor::Enter(lock);

		try {
			getIndex = 0;
			putIndex = 0;

		} finally {
			System::Threading::Monitor::PulseAll(lock);
			System::Threading::Monitor::Exit(lock);
		}
	}

private:
	size_t	bufSize,		//the size of the buffer
			getIndex,		//the index on which the next object to fetch from the buffer
			putIndex,		//the index on which the next object to put in the buffer
			numEntries;		//number of elements in buffer

	T *buf;					// The buffer of objects of type T

	System::Object ^lock;	//Dummy object for locking monitors
};

