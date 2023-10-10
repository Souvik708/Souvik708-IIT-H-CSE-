#include <stdio.h>
#include <vector>
#include <thread>
#include <mutex>
#include <random>
#include <queue>
#include <chrono>
#include <fstream>
#include "CLQ.h"
#include "CLQ.cpp"

using namespace std ;

template <typename T>

class QTester{
public:
    std::vector<double> thrTimes;
    //std::vector<std::chrono::milliseconds> thrTimes;
    QTester(int n, int numOps, double Lambda, double rndLt) : n(n), numOps(numOps), Lambda(Lambda), rndLt(rndLt), qObj(n*numOps){
        //qObj = LockBasedQueue<T>(n*numOps) ;
        thrTimes.resize(n,0) ; // theTimes is a vector which stores the time taken by each thread
    }

    void testThread(int currentThread){
        random_device rd;
        mt19937 generator(rd());
        uniform_int_distribution<int> distribution(1, 100);
        double unifVal, sleepTime  ;
        //auto startTime, stopTime ;
        std::chrono::duration<double> elapsed ; //It is storing time in sec
        //std::chrono::milliseconds elapsed; //It is will the time in miliseconds
        std::default_random_engine uniformObj ;
        std::exponential_distribution<double> expRandObj(Lambda) ;
        T v ;

        // Each thread executes ‘numOps’ operations
        for(int i=0 ; i< numOps ; i++){
            p= uniformObj() ; // will it generate different value every time ?
            if(p<rndLt){    // Enqueue case
                v=distribution(generator) ; // Here we have generate a random number

                auto startTime = chrono::high_resolution_clock::now();
                qObj.enq(v) ;
                auto stopTime = chrono::high_resolution_clock::now();
                elapsed = stopTime - startTime;
                //elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(stopTime - startTime);
            }
            else{
                auto startTime = chrono::high_resolution_clock::now();
                v=qObj.deq() ;
                auto stopTime = chrono::high_resolution_clock::now();
                elapsed = stopTime - startTime;
                //elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(stopTime - startTime);
            }
            // store the execution time
            //std::chrono::duration<double> elapsed = stopTime - startTime;
            thrTimes[currentThread] += (elapsed.count())*1000 ;
            //sleep the thread for random in mili seconds
            sleepTime= expRandObj(uniformObj) ;
            std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>(sleepTime * 1000)));
        }

    }
    private:
    int n, numOps ;
    double Lambda, rndLt ;
    LockBasedQueue<T> qObj;
    double p;
} ;

FILE * log_file ;
FILE * Times ;
int n , numOps ;
double Lambda ;
double rndLt ;

int main(){
    double total, avg ;
    std:: ifstream inFile("inp-params.txt",std::ifstream::in);
    if(inFile.fail())
	{
		printf("File not found");
		exit(1);
	}
inFile >> n >> numOps >> rndLt >> Lambda ;
QTester<int> tester(n,numOps,rndLt,Lambda); 
std::vector<thread>threads(n) ;

log_file=fopen("CLQ","w");

for(int i=0 ; i< n ; i++){
    threads[i] = thread([&tester, i]() {
        tester.testThread(i);
    });
}

for(int i=0 ; i< n ; i++){
    threads[i].join() ;
}
for(int i=0 ; i< n ; i++){
    printf("time taken by thread%d= %f mSec \n",i,tester.thrTimes[i]);
    fprintf(log_file,"time taken by thread%d= %f mSec \n",i,tester.thrTimes[i]);
    total += tester.thrTimes[i] ;
}
avg = total/n ;
printf("LockBased Queue Threads Avg execution time: %f msec\n", avg);
fprintf(log_file,"waitFree Queue Threads Avg execution time: %f\n", avg);
fclose(log_file);
}
