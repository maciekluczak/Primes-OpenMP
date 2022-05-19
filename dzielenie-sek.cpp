#include <stdio.h>
#include <omp.h>
#include <time.h>
#include <chrono>
#include <iostream>
#include <vector>

using namespace std;

#define MAX 150000000	//GORNY LIMIT
#define MIN 2			//DOLNY LIMIT


int isprime(int x)
{
	for (int y = 2; y * y <= x; y++)
	{
		if (x % y == 0)
			return 0;
	}

	return 1;
}

int main()
{
	int sum = 0;
	vector<long> primeNumbers;
	primeNumbers.reserve(MAX - MIN + 1);
	long indice = 0;
	int start;

    if (MIN < 2)
	{
		start = 2;
	}
	else {
		start = MIN;
	}

	auto chronostart = chrono::steady_clock::now();
	for (int i = start; i <= MAX; i++)
	{
		if (isprime(i))
		{
			sum++;
			primeNumbers.push_back(i);
		}
	}
	auto chronoend = chrono::steady_clock::now();

	// for (int i = 0; i < indice; i++) {
	// 	printf("%ld ", primeNumbers[i]);
	// }
	// printf("\nNumber of primes numbers: %d", sum);
	cout <<"Processing time: "<< chrono::duration_cast<chrono::milliseconds>(chronoend - chronostart).count() <<endl;
	return 0;
}
