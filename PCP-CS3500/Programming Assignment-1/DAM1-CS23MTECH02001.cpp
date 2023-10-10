#include <stdio.h>
#include <iostream>
#include <atomic>
#include <vector>
#include <thread>
#include <mutex>
#include <fstream>
#include <chrono>

using namespace std ;

FILE * log_file ;
FILE * Times ;
mutex mtx ;
int thread_num = 4;
atomic<int>counter(1) ; 

bool isPrime(int n) {
    if (n <= 1)
        return false;
    for (int i = 2; i * i <= n; i++) {
        if (n % i == 0)
            return false;
    }
    return true;
}

void checkPrimeWithCounter(int num, int threadId) {
    while (true) {
        int current = counter.fetch_add(2); // Atomically increment counter and get previous value
        if (current > num) {
            break; // All numbers have been checked
        }
        if (isPrime(current)) {
            mtx.lock() ;
            fprintf(log_file, "%d(Tid:%d) \n",current,threadId);
            mtx.unlock() ;
        }
    }
}

int main(){
    int num ;
    //std::cout << "Enter the N :" ;
    //cin >> num ;
    //cin >> thread_num ;
    std:: ifstream inFile("inp-params.txt",std::ifstream::in);
    if(inFile.fail())
	{
		printf("File not found");
		exit(1);
	}
    inFile>>num >>thread_num ;

    vector<thread>threads(thread_num) ;
    
    log_file=fopen("DAM1_log_file","w");
    Times=fopen("Times.txt","a");

    auto start = chrono::high_resolution_clock::now();
    fprintf(log_file, "2 \n");
    for(int i=0 ; i< thread_num ; i++){
        threads[i]=thread(checkPrimeWithCounter, num , i) ;
    }

    for(int i=0 ; i< thread_num ; i++){
        threads[i].join() ;
    }

    auto stop = chrono::high_resolution_clock::now();

    auto duration = chrono::duration_cast<chrono::microseconds>(stop - start);
    cout << "Time taken: " << duration.count() << " microseconds" << endl;
    fprintf(Times," %ld \n",duration.count());

    fclose(log_file) ;
    fclose(Times) ;
    return 0 ;
}
