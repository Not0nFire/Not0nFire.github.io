#include <iostream>
#include <atomic>
#include <thread>
#include <mutex>
#include <sstream>

using namespace std;

const int n = 10;
atomic_int _number;
atomic_int _next;
atomic_int _turn[n];
int numThreads;
mutex coutMutex;

ostringstream data;

void func() {
	int i = numThreads++;

	coutMutex.lock();
	cout << "Thread " << i << " reporting in." << endl;
	coutMutex.unlock();
	
	this_thread::sleep_for(chrono::milliseconds(rand() % 500 + 500));

	while (true)
	{
		//Take a ticket
		_turn[i] = _number.fetch_add(1);

		//Using a mutex for output so that threads don't uppercut each other on the console.
		coutMutex.lock();
		cout << "t" << i << "\tturn: " << _turn[i] << endl;
		coutMutex.unlock();

		//Slow down the program so that we can read the console.
		this_thread::sleep_for(chrono::milliseconds(rand() % 500 + 500));

		while (_turn[i] != _next)
		{
			continue;
		}

		coutMutex.lock();
		cout << "t" << i << "\t+CS" << endl;
		coutMutex.unlock();

		//critical section
		data << " data_t" << i;

		//exit protocol
		_next += 1;

		coutMutex.lock();
		cout << data.str() << endl;
		cout << "t" << i << "\tnext = " << _next << endl;
		coutMutex.unlock();
	}
}


int _tmain(int argc, _TCHAR* argv[])
{
	srand(time(NULL));

	data = ostringstream();

	numThreads = 0;
	_number = 1;
	_next = 1;
	for (int i = 0; i < n; i++)
	{
		_turn[i] = 0;
	}

	thread t1 = thread(func);
	thread t2 = thread(func);
	//thread t3 = thread(func);
	//thread t4 = thread(func);
	//thread t5 = thread(func);

	t1.join();
	t2.join();
	//t3.join();
	//t4.join();
	//t5.join();
	
	return 0;
}