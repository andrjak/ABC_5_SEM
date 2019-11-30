#include "pch.h"

#include <iostream>
#include <thread>
#include <mutex>
#include <atomic>

using namespace std;

typedef int8_t byte;
unsigned int arraySize = 1024 * 1024;
atomic<unsigned int> counterAtomic = 0;
unsigned int counterMutex = 0;
mutex m;

byte *Task1(byte *array, unsigned int NumThreads, void(*Solver) (byte *array), void(*message) (unsigned int NumThreads, long long duration));
void Message(unsigned int NumThreads, long long duration);
void ArrayClear(byte *array, unsigned int size);
void ShowArray(byte *array, unsigned int size);
void SolveTask1AtomicSleep(byte *array);
void SolveTask1MutexSleep(byte *array);
void SolveTask1Atomic(byte *array);
void SolveTask1Mutex(byte *array);

int main()
{
	unsigned int NumTreads[] = { 1, 2, 4, 8, 16, 32, 64 }; // Количество потоков	
	byte *array = new byte[arraySize]; // Массив байтов
	ArrayClear(array, arraySize);

	cout << "\nAtomic:\n";
	for (int i = 0; i < 7; i++)
		Task1(array, NumTreads[i], SolveTask1Atomic, Message);

	cout << "\nMutex:\n";
	for (int i = 0; i < 7; i++)
		Task1(array, NumTreads[i], SolveTask1Mutex, Message);

	cout << "\nAtomic Sleep:\n";
	for (int i = 0; i < 7; i++)
		Task1(array, NumTreads[i], SolveTask1AtomicSleep, Message);

	cout << "\nMutex Sleep:\n";
	for (int i = 0; i < 7; i++)
		Task1(array, NumTreads[i], SolveTask1MutexSleep, Message);
}

// Параметры: Указатель на массив, Количество потоков выполнения, Указатель на функцию решения, Указатель на функцию вывода
byte *Task1(byte *array, unsigned int NumThreads, void(*Solver) (byte *array), void(*message) (unsigned int NumThreads, long long duration))
{
	thread *threadArray = new thread[NumThreads];

	chrono::high_resolution_clock::time_point t1 = chrono::high_resolution_clock::now(); // Начало отсчёта времени

	for (thread *i = threadArray; i < threadArray + NumThreads; i++)
	{
		*i = thread(Solver, array);
		i->join();
	}

	chrono::high_resolution_clock::time_point t2 = chrono::high_resolution_clock::now();
	Message(NumThreads, chrono::duration_cast<chrono::milliseconds>(t2 - t1).count());
	counterAtomic = 0;
	counterMutex = 0;
	return array;
}

void SolveTask1Atomic(byte *array)
{
	unsigned int counter = 0;
	while (true)
	{
		counter = counterAtomic++;
		if (counter < arraySize)
			(*(array + counter))++;
		else
			return;
	}
}

void SolveTask1AtomicSleep(byte *array)
{
	unsigned int counter = 0;
	while (true)
	{
		counter = counterAtomic++;
		if (counter < arraySize)
		{
			(*(array + counter))++;
			this_thread::sleep_for(chrono::nanoseconds(10));
		}
		else
			return;
	}
}

void SolveTask1Mutex(byte *array)
{
	unsigned int counter = 0;
	while (true)
	{
		m.lock();
		counter = counterMutex++;
		m.unlock();
		if (counter < arraySize)
			(*(array + counter))++;
		else
			break;
	}
}

void SolveTask1MutexSleep(byte *array)
{
	unsigned int counter = 0;
	while (true)
	{
		m.lock();
		counter = counterMutex++;
		m.unlock();
		if (counter < arraySize)
		{
			(*(array + counter))++;
			this_thread::sleep_for(chrono::nanoseconds(10));
		}
		else
			break;
	}
}

void ArrayClear(byte *array, unsigned int size)
{
	for (unsigned int i = 0; i < size; i++)
		(*(array + i)) = 0;
}

void ShowArray(byte *array, unsigned int size)
{
	for (unsigned int i = 0; i < size; i++)
		cout << (*(array + i)) << " ";
}

void Message(unsigned int NumThreads, long long duration)
{
	cout << "Number of threads: " << NumThreads << ", Time: " << duration << "\n";
}