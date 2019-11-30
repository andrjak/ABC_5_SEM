#pragma once
#include "pch.h"
//#include <iostream>
//#include <queue>
//#include <mutex>
//#include <thread>
//#include <condition_variable>
//#include "limited_queue_mutex.h"

//using namespace std;

//template<typename T>
//limited_queue_mutex<T>::limited_queue_mutex()
//{
//}
//
//template<typename T>
//limited_queue_mutex<T>::limited_queue_mutex(unsigned int size)
//{
//	_size = size;
//}
//
//
//template<typename T>
//limited_queue_mutex<T>::~limited_queue_mutex()
//{
//}
//
//template<typename T>
//void limited_queue_mutex<T>::push(T val)
//{
//	unique_lock<mutex> mutex_flag(_mutex);
//	if (_size == _queue.size)
//	{		
//		_write_condition.wait(mutex_flag);
//		_read_condition.notify_one();
//	}
//	_queue.push(val);
//	_read_condition.notify_one();
//}
//
//template<typename T>
//bool limited_queue_mutex<T>::pop(T& val)
//{
//	unique_lock<mutex> mutex_flag(_mutex);
//
//	if (_queue.empty())
//	{
//		_read_condition.wait_for(mutex_flag, chrono::milliseconds(1));
//		if (_queue.empty())	
//			return false;
//	}
//
//	val = _queue.front();
//	_queue.pop();
//	_write_condition.notify_one();
//	return true;
//}
