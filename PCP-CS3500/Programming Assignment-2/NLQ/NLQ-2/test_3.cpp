#include <stdio.h>
#include <vector>
#include <thread>
#include <mutex>
#include <random>
#include <queue>
#include <chrono>
#include <fstream>
#include "NLQ.h"
#include "NLQ_deterministic.cpp"

using namespace std;

template <typename T>
class QTester {
public:
    std::vector<double> thrTimes;
    std::vector<double> EnqthrTimes;
    std::vector<double> DeqthrTimes;
    QTester(int n, int numOps, double Lambda, double rndLt) : n(n), numOps(numOps), Lambda(Lambda), rndLt(rndLt), qObj() {
        thrTimes.resize(n, 0); // theTimes is a vector which stores the time taken by each thread
        EnqthrTimes.resize(n, 0) ; // this keeps the records for the enqueue operations
        DeqthrTimes.resize(n, 0) ; // this keeps the records for the dequeue operations
    }

    void testThread(int currentThread) {
        random_device rd;
        mt19937 generator(rd());
        uniform_int_distribution<int> distribution(1, 100);
        double unifVal, sleepTime;
        std::chrono::duration<double> elapsed;
        std::chrono::duration<double> enq;
        std::chrono::duration<double> deq;
        std::default_random_engine uniformObj;
        std::exponential_distribution<double> expRandObj(Lambda);
        T* v;

        // Each thread executes ‘numOps’ operations
        for (int i = 0; i < numOps; i++) {
            p = uniformObj(); // will it generate different value every time ?
            if (p < rndLt) {    // Enqueue case
                v = new T(distribution(generator)); // Here we have generate a random number

                auto startTime = chrono::high_resolution_clock::now();
                qObj.enq(v);
                auto stopTime = chrono::high_resolution_clock::now();
                elapsed = stopTime - startTime;
                enq = stopTime - startTime;
                EnqthrTimes[currentThread] += (enq.count())*1000 ;
            } else {
                auto startTime = chrono::high_resolution_clock::now();
                v = qObj.deq();
                auto stopTime = chrono::high_resolution_clock::now();
                elapsed = stopTime - startTime;
                deq = stopTime - startTime; 
                DeqthrTimes[currentThread] += (deq.count())*1000 ;
                delete v;
            }
            thrTimes[currentThread] += (elapsed.count())*1000;
            sleepTime = expRandObj(uniformObj);
            std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>(sleepTime * 1000)));
        }
    }

private:
    int n, numOps;
    double Lambda, rndLt;
    HWQueue<T> qObj; 
    double p;
};

FILE* log_file;
FILE* Times;
int n, numOps;
double Lambda;
double rndLt;

int main() {
    double total, avg, total_enq, total_deq;
    std::ifstream inFile("inp-params.txt", std::ifstream::in);
    if (inFile.fail()) {
        printf("File not found");
        exit(1);
    }
    inFile >> n >> numOps >> rndLt >> Lambda;
    QTester<int> tester(n, numOps, rndLt, Lambda);
    std::vector<thread> threads(n);

    log_file=fopen("NLQ_deterministic","w");

    for (int i = 0; i < n; i++) {
        threads[i] = thread([&tester, i]() {
            tester.testThread(i);
        });
    }

    for (int i = 0; i < n; i++) {
        threads[i].join();
    }

    for (int i = 0; i < n; i++) {
        total_deq += tester.DeqthrTimes[i] ;
        total_enq += tester.EnqthrTimes[i] ;
        fprintf(log_file,"time taken by thread%d= %f mSec \n",i,tester.thrTimes[i]);
        printf("time taken by thread%d= %f mSec \n",i,tester.thrTimes[i]);
        total += tester.thrTimes[i];
    }

    avg = total / n;
    fprintf(log_file,"Total time taken bu Enqueue Oparation: %f ms\n",total_enq) ;
    fprintf(log_file,"Total time taken bu Dequeue Oparation: %f ms\n",total_deq) ;
    fprintf(log_file,"waitFree Queue Threads Avg execution time: %f\n", avg);
    fprintf(log_file,"waitFree Queue Avg of enq and deq execution time: %f\n",(total_deq+total_deq)/2);

    printf("Total time taken bu Enqueue Oparation: %f ms\n",total_enq) ;
    printf("Total time taken bu Dequeue Oparation: %f ms\n",total_deq) ;
    printf("waitFree Queue Threads Avg execution time: %f\n", avg);
    printf("waitFree Queue Avg of enq and deq execution time: %f\n",(total_deq+total_deq)/2);
    
}

