#include <stdio.h>
#include <omp.h>
#include <time.h>
#include <chrono>
#include <iostream>
#include <vector>
#include <math.h>

using namespace std;

#define MAX 150000000
#define MIN 2


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
	long start;
	int bound = int(sqrt(MAX));
	bool* isNumberPrime = new bool[MAX + 1] {false};

	vector<long> checkingPrimes;
    checkingPrimes.reserve(bound);

    
    for (int i = 2; i <= bound; i++)
    {
        if (isprime(i))
        {
            checkingPrimes.push_back(i);
        }
        
    }
    int startingPrimesCount = checkingPrimes.size();

	auto chronostart = chrono::steady_clock::now();

	for (int i = 0; i < startingPrimesCount; i++)
	{
		int multiple = checkingPrimes[i] * 2;

		while (multiple <= MAX)
		{
			isNumberPrime[multiple] = true;
			multiple += checkingPrimes[i];
		}
		
	}
	

	auto chronoend = chrono::steady_clock::now();
	int count = 0;
	vector<long> primeNumbers;
	primeNumbers.reserve(bound);

	for (long i = MIN; i <= MAX; i++)
	{
		if (!isNumberPrime[i])
		{
			primeNumbers.push_back(i);
			count ++;
		}
	}
	cout << endl << "Primes: "<< count <<endl;
	cout <<"Processing time: "<< chrono::duration_cast<chrono::milliseconds>(chronoend - chronostart).count() <<endl;

	return 0;
}