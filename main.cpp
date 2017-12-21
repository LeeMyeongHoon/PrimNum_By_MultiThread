#include <ctime>
#include <Windows.h>
#include <iostream>
#include <iomanip>
#include <cmath>
#include <cassert>
#include <thread>
#include <atomic>
#include <chrono>
#include <mutex>

int GetPrimeCount_no_Thread(int num);
int GetPrimeCount_Thread(int num);

bool IsPrime(int num);
void PrimeCounting(int lastNum, int index);


int main()
{
	std::cout << std::fixed << std::setprecision(15);

	int num;
	std::cout << "범위 지정 (8이상): ";
	std::cin >> num;

	std::cout << "쓰레드 : ";
	auto startTime = std::chrono::steady_clock::now();
	std::cout << GetPrimeCount_Thread(num) << "\n";
	auto endTime = std::chrono::steady_clock::now();
	auto elapsedTime = endTime - startTime;
	std::cout << (long double)elapsedTime.count() / 1000000000 << "\n\n"; //

	std::cout << "일반 : ";
	auto startTime2 = std::chrono::steady_clock::now();
	std::cout << GetPrimeCount_no_Thread(num) << '\n';
	auto elapsedTime2 = std::chrono::steady_clock::now() - startTime2;
	std::cout << (long double)elapsedTime2.count() / 1000000000 << "\n\n";


	return 0;
}

int share;
static const int THREAD_COUNT = 8;
int counts[THREAD_COUNT] = { 0 };

int GetPrimeCount_Thread(int lastNum)
{
	share = 1;

	std::thread thrds[THREAD_COUNT];
	for (int i = 0; i < THREAD_COUNT; i++)
	{
		thrds[i] = std::thread(PrimeCounting, lastNum, i);
	}

	int totalCount = 0;
	for (int i = 0; i < THREAD_COUNT; i++)
	{
		thrds[i].join();
		totalCount += counts[i];
	}

	return totalCount;
}


void PrimeCounting(int lastNum, int index)
{
	static std::mutex mtx;
	for (;;)
	{
		mtx.lock();
		int testNum = share;
		++share;
		mtx.unlock();

		if (testNum > lastNum)
			return;

		if (IsPrime(testNum))
		{
			++counts[index];
		}
	}
}


bool IsPrime(int num)
{
	if (num == 1)
	{
		return false;
	}

	if (num == 2)
	{
		return true;
	}
	int sqrt = std::sqrt(num);

	for (int divider = 3; divider <= sqrt; divider++)
	{
		if (num % divider == 0)
		{
			return false;
		}
	}
	return true;
}



int GetPrimeCount_no_Thread(int num)
{
	int count = 0;
	for (int testNum = 1; testNum <= num; testNum++)
	{
		if (IsPrime(testNum))
		{
			++count;
		}
	}

	return count;
}