// 에라토스테네스의 체
// 쓰레드함수 ver2

#define MAIN_3
#ifdef MAIN_3

#include <iostream>
#include <chrono>
#include <iomanip>
#include <thread>
#include <mutex>

const unsigned int THREAD_COUNT = 8;

bool *isChecked;
unsigned int lastNum;
unsigned int share = 2;
unsigned int notPrimeCount = 1;
unsigned int counts[THREAD_COUNT] = { 0 };

std::mutex mtx;


unsigned int original();

unsigned int ThreadVersion();
void Counting(int thrdNum);

int main(void)
{
	isChecked = new bool[1000000000 + 1];
	std::cout << std::fixed << std::setprecision(15);

	std::cout << "범위 지정 (8이상): ";
	std::cin >> lastNum;

	std::cout << "쓰레드 : ";
	auto startTime = std::chrono::steady_clock::now();
	std::cout << original() << "개\n";
	auto endTime = std::chrono::steady_clock::now();
	auto elapsedTime = endTime - startTime;
	std::cout << (long double)elapsedTime.count() / 1000000000 << "\n\n"; //

	delete[] isChecked;

	//isChecked = new bool[1000000000 + 1];

	//std::cout << "일반 : ";
	//auto startTime2 = std::chrono::steady_clock::now();
	//std::cout << ThreadVersion() << "개\n";
	//auto endTime2 = std::chrono::steady_clock::now();
	//auto elapsedTime2 = endTime2 - startTime2;
	//std::cout << (long double)elapsedTime2.count() / 1000000000 << "\n\n"; //

	//delete[] isChecked;

	return 0;
}

unsigned int original()
{
	unsigned int notPrimeCount = 1;	// 1 포함
	unsigned int sqrt1 = sqrt(lastNum);
	for (unsigned int i = 2; i <= sqrt1; ++i)
	{
		if (isChecked[i] == false)
		{
			for (unsigned int j = 2 * i; j <= lastNum; j += i)
			{
				if (isChecked[j] == false)
				{
					isChecked[j] = true;
					notPrimeCount++;
				}
			}
		}
	}
	return lastNum - notPrimeCount;
}


unsigned int ThreadVersion()
{
	std::thread thrds[THREAD_COUNT];

	int notPrimeCount = 1;
	for (int thrdNum = 0; thrdNum < THREAD_COUNT; thrdNum++)
	{
		thrds[thrdNum] = std::thread(Counting, thrdNum);
	}

	for (int thrdNum = 0; thrdNum < THREAD_COUNT; thrdNum++)
	{
		thrds[thrdNum].join();
		notPrimeCount += counts[thrdNum];
	}

	return lastNum - notPrimeCount;
}

void Counting(int thrdNum)
{
	const int offset = thrdNum;
	for (int testNum = thrdNum; testNum <= lastNum; lastNum += offset)
	{
		mtx.lock();
		if (!isChecked[testNum])
		{
			mtx.unlock();
			for (unsigned int multiple = 2 * testNum; multiple <= lastNum; multiple += offset)
			{
				mtx.lock();
				if (isChecked[multiple] == false)
				{
					mtx.unlock();
					isChecked[multiple] = true;
					counts[thrdNum]++;
				}
				else
				{
					mtx.unlock();
				}
			}
		}
		else
		{
			mtx.unlock();
		}
	}
}
#endif // MAIN_3
