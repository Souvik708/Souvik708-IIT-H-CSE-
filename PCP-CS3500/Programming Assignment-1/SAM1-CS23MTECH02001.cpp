#include <stdio.h>
#include <iostream>
#include <thread>
#include <vector>
#include <mutex>
#include <chrono>
#include <fstream>

using namespace std;

FILE * log_file;
FILE * Times;
mutex mtx;
int thread_num = 4;

bool isPrime(int n) {
    if (n <= 1)
        return false;
    for (int i = 2; i * i <= n; i++) {
        if (n % i == 0)
            return false;
    }
    return true;
}

void primePrint(int n) {
    cout << n;
}

//void checkPrimeInRange(int start, int end, int threadId) {
//    for (int i = start; i <= end; i += thread_num) {
//        if (i % 2 == 0 && i != 2) {
//            continue; // Skip even numbers (except 2)
//        }
//        if (isPrime(i)) {
//            mtx.lock();
//            fprintf(log_file, "%d(Tid:%d) \n", i, threadId);
//            // primePrint(i);
//            // cout << "(" << threadId << ")" << " ";
//            mtx.unlock();
//        }
//    }
//}

void checkPrimeInRange(int n, int threadId, int numThreads) {
    for (int numToCheck = 1 + threadId; numToCheck < n; numToCheck += numThreads) {
        if (numToCheck % 2 == 0)
            continue; // Skip even numbers except for 2
        if (isPrime(numToCheck)) {
            mtx.lock();
            fprintf(log_file, "%d(Tid:%d) \n", numToCheck, threadId);
            mtx.unlock();
        }
    }
}

int main() {
    int num;
    // std::cout << "Enter the N :";
    // cin >> num ;
    // cin >> thread_num ;
    std::ifstream inFile("inp-params.txt", std::ifstream::in);
    if (inFile.fail()) {
        printf("File not found");
        exit(1);
    }
    inFile >> num >> thread_num;

    std::vector<thread> threads(thread_num);
    //int range = num / thread_num;

    log_file = fopen("SAM2_log_file", "w");
    Times = fopen("Times.txt", "a");

    auto start = chrono::high_resolution_clock::now();

    for (int i = 0; i < thread_num; i++) {
        threads[i] = thread(checkPrimeInRange, num , i ,thread_num);
    }

    for (int i = 0; i < thread_num; i++) {
        threads[i].join();
    }

    auto stop = chrono::high_resolution_clock::now();

    auto duration = chrono::duration_cast<chrono::microseconds>(stop - start);
    cout << "Time taken: " << duration.count() << " microseconds" << endl;
    fprintf(Times, " %ld \n", duration.count());

    fclose(log_file);
    fclose(Times);
    return 0;
}
