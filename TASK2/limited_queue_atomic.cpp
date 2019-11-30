#pragma once
#include "pch.h"
//#include <queue>
//#include <atomic>
//#include "limited_queue_atomic.h"
//
//template<typename T>
//limited_queue_atomic<T>::limited_queue_atomic()
//{
//}
//
//template<typename T>
//limited_queue_atomic<T>::limited_queue_atomic(unsigned int size)
//{
//	_size = size;
//}
//
//template<typename T>
//limited_queue_atomic<T>::~limited_queue_atomic()
//{
//}
//
//template<typename T>
//void limited_queue_atomic<T>::push(T val)
//{
//	for (;;) 
//	{
//		auto local_end = _end.load();
//		if (local_end == _start + _size)  // Очередь полна
//			continue;
//		if (_end.compare_exchange_strong(local_end, local_end + 1))
//		{
//			_array[local_end] = val;
//			return;
//		}
//	}
//}
//
//template<typename T>
//bool limited_queue_atomic<T>::pop(T& val)
//{
//	for (;;)
//	{
//		auto local_start = _start.load();               
//		if (local_start == _end.load())  // Очередь пуста                  
//		{
//			this_thread::sleep_for(chrono::milliseconds(1));
//			local_start = _start.load();
//			if (local_start == _start.load()) // Очередь по прежнему пуста
//				return false;
//		}
//		if (_start.compare_exchange_strong(local_start, local_start + 1))
//			val = _array[local_start];
//		return true;
//	}
//}
