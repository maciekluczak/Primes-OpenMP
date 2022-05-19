#include <stdio.h>
#include <omp.h>
#include <time.h>
#include <math.h>
#include <sys/resource.h>
#include <vector>
#include <time.h>
#include <iostream>
#include <chrono>

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

int startingPoint(int threadNumber, int maxThreadCount){
    int startPoint = MIN + (MAX - MIN) / maxThreadCount * threadNumber;

    return startPoint;
}

int endingPoint(int threadNumber, int maxThreadCount){
    if (threadNumber < maxThreadCount - 1)
    {
        int endPoint = startingPoint(threadNumber, maxThreadCount) + (MAX - MIN) / maxThreadCount - 1;
        return endPoint;
    }
    else
    {
        return MAX;
    }
    
}


int main()
{
    int maxThreadsCount = 4;
    int bound = int(sqrt(MAX));
    long start;
    vector<long> checkingPrimes;
    checkingPrimes.reserve(bound);

    
    for (int i = 2; i <= bound; i++)
    {
        if (isprime(i))
        {
            checkingPrimes.push_back(i);
            cout << i<<endl;
        }
        
    }
    int startingPrimesCount = checkingPrimes.size();
    
   bool** isNumberPrime = new bool* [maxThreadsCount];
   for (int i = 0; i < maxThreadsCount; i++)
   {
       isNumberPrime[i] = new bool[MAX + 1]{ false };
   }

    auto chronostart = chrono::steady_clock::now();

    #pragma omp parallel num_threads(maxThreadsCount)
    {
        
        int startPoint = startingPoint(omp_get_thread_num(), maxThreadsCount);
        int endPoint = endingPoint(omp_get_thread_num(), maxThreadsCount);

        for (int i = 0; i < startingPrimesCount; i++)
        {
            int multiple = checkingPrimes[i] * 2;
            int power = ((startPoint - checkingPrimes[i] * 2) / checkingPrimes[i]) * checkingPrimes[i];

            if (power > 0)
            {
                multiple += power;
            }

            while (multiple <= endPoint)
            {
                isNumberPrime[omp_get_thread_num()][multiple] = true;
                multiple += checkingPrimes[i];
            }
            
            
        }
        
    }

    long primeNumbers[MAX+1] { 0 };

    for (int i = MIN; i <= MAX; i++)
    {
        int sum = false;
        for (int j = 0; j < maxThreadsCount; j++)
        {
            sum |= isNumberPrime[j][i];
        }
        if (!sum)
        {
            primeNumbers[i] = i;
        }
        
    }
    auto chronoend = chrono::steady_clock::now();
    // for (int i = MIN; i < MAX+1; i++)
    // {
    //     printf("%ld ", primeNumbers[i]);
    // }
    // cout <<"Processing time: "<< chrono::duration_cast<chrono::milliseconds>(chronoend - chronostart).count() <<endl;

 
    delete[] isNumberPrime;

	return 0;
}