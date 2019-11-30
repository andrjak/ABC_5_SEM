#pragma once
#include "pch.h"
//#include <queue>
//#include <mutex>
//#include <thread>
//#include <condition_variable>
//#include "queue_mutex.h"
//
//using namespace std;

//template<typename T>
//queue_mutex<T>::queue_mutex()
//{
//}
//
//template<typename T>
//queue_mutex<T>::~queue_mutex()
//{
//}
//
//template<typename T>
//void queue_mutex<T>::push(T val)
//{
//	unique_lock<mutex> mutex_flag(_mutex);
//	queue_mutex<T>::_queue.push(val);
//}
//
//template<typename T>
//bool queue_mutex<T>::pop(T& val)
//{
//	unique_lock<mutex> mutex_flag(_mutex);
//
//	if (_queue.empty)
//	{
//		mutex_flag.unlock();
//		this_thread::sleep_for(chrono::milliseconds(1));
//		if (_queue.empty)
//			return false;
//	}
//
//	val = _queue.front();
//	_queue.pop();
//	return true;
//}
