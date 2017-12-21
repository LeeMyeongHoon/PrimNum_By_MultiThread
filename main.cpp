#include <iostream>
#include <chrono>

bool *IsNotPrime;
int getChe1(int num)
{
	int count = 1;	// 2 포함
	IsNotPrime = new bool[num + 1];
	for (int i = 3; i <= sqrt(num); i += 2)
	{
		if (IsNotPrime[i] == true) // 이미 체크된 수의 배수는 확인하지 않는다
			continue;

		for (int j = i; j <= num; j += i)
		{
			IsNotPrime[j] = true;
		}
	}
}
int main(void) {



	delete[] IsNotPrime;
	return 0;
}

