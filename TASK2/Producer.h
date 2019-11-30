#include "pch.h"
#include <thread>

using namespace std;

typedef uint8_t byte;

template<typename Q>
class Producer
{
private:
	unsigned long long _counter;
	unsigned int _producerNum;
	Q* _queue;

public:
	thread* _threads;
	Producer(unsigned long long counter, unsigned int producerNum, Q* queue);
	void write();
	void start();
	~Producer();
};

template<typename Q>
Producer<Q>::Producer(unsigned long long counter, unsigned int producerNum, Q* queue)
{
	_counter = counter * producerNum;
	_producerNum = producerNum;
	_queue = queue;
}

template<typename Q>
void Producer<Q>::write()
{
	for (int i = 0; i < _counter; i++) 
		_queue->push(1);
}

template<typename Q>
void Producer<Q>::start()
{
	_threads = new thread[_producerNum];
	for (int i = 0; i < _producerNum; i++)
		_threads[i] = thread(&Producer::write, this);
}

template<typename Q>
Producer<Q>::~Producer()
{
}