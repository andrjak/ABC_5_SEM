#include <queue>
#include <atomic>
#include <vector>

using namespace std;
typedef uint8_t byte;

class limited_queue_atomic
{
private:
	unsigned int _size;
	atomic<byte>* _vector;
	alignas(64) atomic<int> _end;
	alignas(64) atomic<int> _start;

public:
	limited_queue_atomic(unsigned int size);
	~limited_queue_atomic();
	void push(byte val);
	bool pop(byte& val);
};

limited_queue_atomic::limited_queue_atomic(unsigned int size)
{
	_size = size;
	_vector = new atomic<byte>[size];
	_start = 0;
	_end = 0;
	for (int i = 0; i < size; i++)
		_vector[i] = 0;
}

limited_queue_atomic::~limited_queue_atomic()
{
}

// Вариант как в книге
void limited_queue_atomic::push(byte val)
{
	for (;;)
	{
		auto local_end = _end.load(); // Извлекаем значение _end
		byte temp = _vector[local_end % _size];
		if (local_end != _end) // Кто-то изменил счётчик, значит данные уже не актуальны 
			continue;
		if (local_end == _start + _size)  // Очередь полна
			continue;
		if (temp == 0)
		{
			if (_vector[local_end % _size].compare_exchange_strong(temp, val))
			{
				_end.compare_exchange_strong(local_end, local_end + 1); // Если значение было изменено true иначе false
				return;
			}
		}
		else _end.compare_exchange_strong(local_end, local_end + 1);
	}
}

bool limited_queue_atomic::pop(byte& val)
{
	for (;;)
	{
		auto local_start = _start.load(); // Извлекаем значение _start
		byte temp = _vector[local_start % _size];
		if (local_start != _start) // Кто-то изменил счётчик, значит данные уже не актуальны 
			continue;
		if (local_start == _end.load())  // Очередь пуста                  
		{
			this_thread::sleep_for(chrono::milliseconds(1));
			local_start = _start.load(); // Сново извлекаем _start
			if (local_start == _end.load()) // Очередь по прежнему пуста
				return false; // Значение не полученго
			else if (local_start != _start)
				continue;
		}

		if (temp != 0)
		{
			if (_vector[local_start % _size].compare_exchange_strong(temp, 0))
			{
				_start.compare_exchange_strong(local_start, local_start + 1); // Если значение было изменено true иначе false
				val = temp;
				return true; // Получили значение 
			}
		}
		else _start.compare_exchange_strong(local_start, local_start + 1);
	}
}


//// Вариант c обнуления не совсем как в книге 
//void limited_queue_atomic::push(byte val)
//{
//	for (;;)
//	{
//		auto local_end = _end.load(); // Извлекаем значение _end
//		byte temp = _vector[local_end % _size];
//		if (local_end != _end) // Кто-то изменил счётчик, значит данные уже не актуальны 
//			continue;
//		if (local_end == _start + _size)  // Очередь полна
//			continue;
//		
//		if (temp == 0)
//		{
//			if (_end.compare_exchange_strong(local_end, local_end + 1)) // Если значение было изменено true иначе false
//			{
//				_vector[local_end % _size] = val;
//				return;
//			}
//		}
//		else _end.compare_exchange_strong(local_end, local_end + 1);
//	}
//}
//
//bool limited_queue_atomic::pop(byte& val)
//{
//	for (;;)
//	{
//		auto local_start = _start.load(); // Извлекаем значение _start
//		byte temp = _vector[local_start % _size];
//		if (local_start != _start) // Кто-то изменил счётчик, значит данные уже не актуальны 
//			continue;
//		if (local_start == _end.load())  // Очередь пуста                  
//		{
//			this_thread::sleep_for(chrono::milliseconds(1));
//			local_start = _start.load(); // Сново извлекаем _start
//			if (local_start == _end.load()) // Очередь по прежнему пуста
//				return false; // Значение не полученго
//			else
//				continue;
//		}
//		if (temp != 0)
//		{
//			if (_start.compare_exchange_strong(local_start, local_start + 1)) // Если значение было изменено true иначе false
//			{
//				_vector[local_start % _size] = 0;
//				val = temp;
//				return true; // Получили значение 
//			}
//		}
//		else _start.compare_exchange_strong(local_start, local_start + 1);
//	}
//}

//// Вариант без обнуления
//void limited_queue_atomic::push(byte val)
//{
//	for (;;)
//	{
//		auto local_end = _end.load(); // Извлекаем значение _end
//		if (local_end == _start + _size)  // Очередь полна
//			continue;
//
//		if (_end.compare_exchange_strong(local_end, local_end + 1)) // Если значение было изменено true иначе false
//		{
//			_vector[local_end % _size] = val;
//			return;
//		}
//	}
//}
//
//bool limited_queue_atomic::pop(byte& val)
//{
//	for (;;)
//	{
//		auto local_start = _start.load(); // Извлекаем значение _start
//		byte temp = _vector[local_start % _size];
//		if (local_start != _start) // Кто-то изменил счётчик, значит данные уже не актуальны 
//			continue;
//		if (local_start == _end.load())  // Очередь пуста                  
//		{
//			this_thread::sleep_for(chrono::milliseconds(1));
//			local_start = _start.load(); // Сново извлекаем _start
//			if (local_start == _end.load()) // Очередь по прежнему пуста
//				return false; // Значение не полученго
//			else
//				continue;
//		}
//
//		if (_start.compare_exchange_strong(local_start, local_start + 1)) // Если значение было изменено true иначе false
//		{
//			//_vector[local_start % _size] = 0;
//			val = temp;
//			return true; // Получили значение 
//		}
//	}
//}
