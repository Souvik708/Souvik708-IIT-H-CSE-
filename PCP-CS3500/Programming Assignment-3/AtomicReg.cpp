#include <iostream>
#include <thread>
#include <vector>
#include <climits>
#include <chrono>
#include <random>
#include <fstream>
#include <iomanip> 
#include <cstdio>
#include <atomic>

using namespace std ;

FILE * log_file ;
int capacity;
int numOps ;
double Lambda ;

void printTimestamp(std::chrono::high_resolution_clock::time_point timestamp, FILE* file) {
    auto duration = std::chrono::duration_cast<std::chrono::seconds>(timestamp.time_since_epoch());
    auto hours = std::chrono::duration_cast<std::chrono::hours>(duration).count() % 24;
    duration -= std::chrono::hours(hours);
    auto minutes = std::chrono::duration_cast<std::chrono::minutes>(duration).count() % 60;
    duration -= std::chrono::minutes(minutes);
    auto seconds = duration.count() % 60;

    fprintf(file, "%02ld:%02ld:%02ld", hours, minutes, seconds);
}

const double p = 0.5; // Probability for read operation

void testAtomic(int id, atomic<int>& shVar, int numOps) {
    FILE* file = fopen("logfile(Atomic).txt", "a"); 
    if (file == nullptr) {
        perror("Error opening file");
        return;
    }

    int localVar;
    std::random_device rd;
    std::default_random_engine generator(rd());
    std::bernoulli_distribution distribution(p);

    for (int i = 0; i < numOps; ++i) {
        auto reqTime = std::chrono::high_resolution_clock::now();
        fprintf(file, "%d action requested at ", i);
        printTimestamp(reqTime, file);
        fprintf(file, " by thread %d\n", id);

        bool action = distribution(generator);
        if (action) {
            localVar = shVar.load();
            fprintf(file, "Value read: %d\n", localVar);
        } else {
            localVar = numOps * id;
            shVar.store(localVar);
            fprintf(file, "Value written: %d\n", localVar);
        }

        auto complTime = std::chrono::high_resolution_clock::now();
        fprintf(file, "%d action %s completed at ", i, (action ? "read" : "write"));
        printTimestamp(complTime, file);
        fprintf(file, " by thread %d\n", id);


        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    fclose(file); 
}

int main(){
    std:: ifstream inFile("inp-params.txt",std::ifstream::in);
    if(inFile.fail()) {
		printf("File not found");
		exit(1);
	}
    inFile >> capacity >> numOps >> Lambda ;
    log_file=fopen("AtomicRegister","w");
    int initialValue=0 ;
    atomic <int> shVar(0);

    vector<thread>threads ;
    
    auto start = chrono::high_resolution_clock::now();

    for (int i = 0; i < capacity; ++i) {
        threads.emplace_back(testAtomic, i, std::ref(shVar), numOps);
 
    }

    for (auto& thread : threads) {
        thread.join();
    }
    auto stop = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds>(stop - start);

    cout << "Time taken: " << duration.count() << " milliseconds" << endl;

    return 0 ;
}
