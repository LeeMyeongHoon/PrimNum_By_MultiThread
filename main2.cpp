// 일반 완전탐색?

#if MAIN2
#include <ctime>
#include <Windows.h>
#include <iostream>
#include <iomanip>
#include <cmath>
#include <cassert>
#include <thread>a
#include <atomic>
#include <chrono>
#include <mutex>


static const unsigned int THREAD_COUNT = 8;


unsigned int share = 3;
unsigned int counts[THREAD_COUNT] = { 0 };
unsigned int lastNum;


unsigned int GetPrimeCount_no_Thread();
unsigned int GetPrimeCount_Thread();

bool IsPrime(unsigned int num);
void PrimeCounting(unsigned int index);


int main()
{
	std::cout << std::fixed << std::setprecision(15);

	std::cout << "범위 지정 (8이상): ";
	std::cin >> lastNum;

	std::cout << "쓰레드 : ";
	auto startTime = std::chrono::steady_clock::now();
	std::cout << GetPrimeCount_Thread() << "\n";
	auto endTime = std::chrono::steady_clock::now();
	auto elapsedTime = endTime - startTime;
	std::cout << (long double)elapsedTime.count() / 1000000000 << "\n\n"; //

	std::cout << "일반 : ";
	auto startTime2 = std::chrono::steady_clock::now();
	std::cout << GetPrimeCount_no_Thread() << '\n';
	auto elapsedTime2 = std::chrono::steady_clock::now() - startTime2;
	std::cout << (long double)elapsedTime2.count() / 1000000000 << "\n\n";


	return 0;
}


unsigned int GetPrimeCount_Thread()
{
	unsigned int totalCount = 1;
	std::thread thrds[THREAD_COUNT];
	for (unsigned int i = 0; i < THREAD_COUNT; i++)
	{
		thrds[i] = std::thread(PrimeCounting, i);
	}

	for (unsigned int i = 0; i < THREAD_COUNT; i++)
	{
		thrds[i].join();
		totalCount += counts[i];
	}

	return totalCount;
}


void PrimeCounting(unsigned int index)
{
	static std::mutex mtx;
	for (;;)
	{
		mtx.lock();
		unsigned int testNum = share;
		share++;
		mtx.unlock();

		if (testNum > lastNum)
			return;

		if (IsPrime(testNum))
		{
			++counts[index];
		}
	}
}


bool IsPrime(unsigned int num)
{
	if (num % 2 == 0)
	{
		return false;
	}

	unsigned int sqrt = std::sqrt(num);
	for (unsigned int divider = 3; divider <= sqrt; divider += 2)
	{
		if (num % divider == 0)
		{
			return false;
		}
	}
	return true;
}

unsigned int GetPrimeCount_no_Thread()
{
	unsigned int count = 1;
	for (unsigned int testNum = 3; testNum <= lastNum; testNum += 2)
	{
		if (IsPrime(testNum))
		{
			++count;
		}
	}

	return count;
}
#endif // MAIN_@
