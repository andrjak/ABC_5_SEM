#include "pch.h"
#include <thread>

using namespace std;

typedef uint8_t byte;

template<typename Q>
class Consumer
{
private:
	unsigned long long _counter;
	unsigned int _consumerNum;
	Q* _queue;

public:
	thread* _threads;
	Consumer(unsigned int consumerNum, Q* queue);
	void read();
	unsigned long long getCounter();
	void start();
	~Consumer();
};

template<typename Q>
Consumer<Q>::Consumer(unsigned int consumerNum, Q* queue)
{
	_counter = 0;
	_consumerNum = consumerNum;
	_queue = queue;
}

template<typename Q>
void Consumer<Q>::read()
{
	bool flag = true;
	byte value;
	while (flag)
	{
		flag = _queue->pop(value);
		if (flag)
			_counter += value;
	}
}

template<typename Q>
unsigned long long Consumer<Q>::getCounter()
{
	return _counter;
}

template<typename Q>
void Consumer<Q>::start()
{
	_threads = new thread[_consumerNum];
	for (int i = 0; i < _consumerNum; i++)
		_threads[i] = thread(&Consumer::read, this);
}

template<typename Q>
Consumer<Q>::~Consumer()
{
}