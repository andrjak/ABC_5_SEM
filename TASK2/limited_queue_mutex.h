#include <iostream>
#include <queue>
#include <mutex>
#include <thread>
#include <condition_variable>

using namespace std;
typedef uint8_t byte;

class limited_queue_mutex
{
private:
	queue<byte> _queue;
	mutex _mutex;
	unsigned int _size;
	condition_variable _read_condition;
	condition_variable _write_condition;

public:
	limited_queue_mutex(unsigned int size);
	~limited_queue_mutex();
	void push(byte val);
	bool pop(byte& val);
};

limited_queue_mutex::limited_queue_mutex(unsigned int size)
{
	_size = size;
}

limited_queue_mutex::~limited_queue_mutex()
{
}

void limited_queue_mutex::push(byte val)
{
	unique_lock<mutex> mutex_flag(_mutex);
	if (_size == _queue.size()) // Очередь заполнена 
	{
		_write_condition.wait(mutex_flag); // Ждём освобождения
		_read_condition.notify_one(); // Разблокируем один поток для чтения
	}
	_queue.push(val); // Если есть свободное место записываем 
	_read_condition.notify_one(); // Разблокируем один поток для чтения
}

bool limited_queue_mutex::pop(byte& val)
{
	unique_lock<mutex> mutex_flag(_mutex);

	if (_queue.empty())
	{
		_read_condition.wait_for(mutex_flag, chrono::milliseconds(1)); // если очередь пуста ждём пока поток не разбудят или 1 мс
		if (_queue.empty())
			return false;
	}

	val = _queue.front();
	_queue.pop();
	_write_condition.notify_one();
	return true;
}

