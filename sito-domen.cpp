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

long* primesToCheck(long *tab, long start, long bound){
    long indice = 0;

    for (long i = start; i <= bound; i++)
    {
        if (isprime(i))
        {
            tab[indice] = i;
            indice++;
        }
    }
    return tab;
}

int main()
{
    
    clock_t cstart, cstop;
    int maxThreadsCount = 4;
    int threadsCount = omp_get_num_threads();
    int bound = int(sqrt(MAX));
    long start;
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
   
   bool** isNumberPrime = new bool* [maxThreadsCount];
   for (int i = 0; i < maxThreadsCount; i++)
   {
       isNumberPrime[i] = new bool[MAX + 1]{ false };
   }
   
   


    auto chronostart = chrono::steady_clock::now();

    #pragma omp parallel num_threads(maxThreadsCount)
    {
    #pragma omp for schedule(dynamic)
        for (int i = 0; i < startingPrimesCount; i++)
        {
            int multiple = checkingPrimes[i] * 2;

            while (multiple <= MAX)
            {
                isNumberPrime[omp_get_thread_num()][multiple] = true;
                multiple += checkingPrimes[i];                
            }
            
        }
        
    }
    auto chronoend = chrono::steady_clock::now();

    long primeNumbers[MAX+1] { 0 };


    for (int i = MIN; i < MAX; i++)
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
    
    // for (int i = MIN; i < MAX; i++)
    // {
    //     printf("%ld ", primeNumbers[i]);
    // }
    
    cout <<"Processing time: "<< chrono::duration_cast<chrono::milliseconds>(chronoend - chronostart).count() <<endl;
 
    delete[] isNumberPrime;

	return 0;
}