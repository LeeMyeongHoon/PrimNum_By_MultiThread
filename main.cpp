#include <ctime>
#include <Windows.h>
#include <iostream>
#include <iomanip>
#include <cmath>
#include <cassert>
#include <thread>
#include <atomic>
#include <chrono>


size_t GetPrimeCount_no_Thread(size_t num);
size_t GetPrimeCount_Thread(size_t num);

bool IsPrime(size_t num);
void PrimeCounting(size_t lastNum);

int main()
{
	std::cout << std::fixed << std::setprecision(15);

	size_t num;
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

static const int THREAD_COUNT = 8;
std::atomic<size_t> testNum;
std::atomic<size_t> totalCount;
size_t GetPrimeCount_Thread(size_t lastNum)
{
	totalCount = 0;
	testNum = 1;

	std::thread thrds[THREAD_COUNT];
	for (int i = 0; i < THREAD_COUNT; i++)
	{
		thrds[i] = std::thread(PrimeCounting, lastNum);
	}

	for (int i = 0; i < THREAD_COUNT; i++)
	{
		thrds[i].join();
	}

	return totalCount.load();
}

void PrimeCounting(size_t lastNum)
{
	for (; testNum.load() <= lastNum; ++testNum)
	{
		if (IsPrime(testNum.load()))
		{
			totalCount++;
		}
	}
}


bool IsPrime(size_t num)
{
	if (num == 1)
	{
		return false;
	}

	if (num == 2)
	{
		return true;
	}
	size_t sqrt = std::sqrt(num);

	for (size_t divider = 3; divider <= sqrt; divider++)
	{
		if (num % divider == 0)
		{
			return false;
		}
	}
	return true;
}



size_t GetPrimeCount_no_Thread(size_t num)
{
	size_t count = 0;
	for (size_t testNum = 1; testNum <= num; testNum++)
	{
		if (IsPrime(testNum))
		{
			++count;
		}
	}

	return count;
}