#include <iostream>
#include <thread>
#include <vector>
#include <climits>
#include <chrono>
#include <random>
#include <fstream>
#include <cstdio>
#include <iomanip> 

using namespace std ;

template <typename T>
class StampedValue {
public:
    long stamp;
    T value;

    StampedValue(T init){
        stamp=0 ;
        value= init ;
    }

    StampedValue(long stamp1, T value1){
        stamp= stamp1 ;
        value= value1 ;
    }

    static StampedValue<T> max(StampedValue x, StampedValue y) {
        return x.stamp > y.stamp ? x : y;
    }

    static StampedValue MIN_VALUE ;

};

template <typename T>
StampedValue<T> StampedValue<T>::MIN_VALUE(INT_MIN);


template <typename T>
class AtomicMRMWRegister {
private:
    std::vector<StampedValue<T>> a_table; // vector of atomic MRSW registers

public:
    int ThreadID ;
    AtomicMRMWRegister(int capacity, T init) : a_table(capacity, StampedValue<T>(init)) {}

    void write(T value, int ThreadID) {
        int me = ThreadID ;
        StampedValue<T> max = StampedValue<T>::MIN_VALUE;
        for (int i = 0; i < a_table.size(); ++i) {
            max = StampedValue<T>::max(max, a_table[i]);
        }
        a_table[me] = StampedValue<T>(max.stamp + 1, value);
    }

    T read() {
        StampedValue<T> max = StampedValue<T>::MIN_VALUE;
        for (int i = 0; i < a_table.size(); ++i) {
            max = StampedValue<T>::max(max, a_table[i]);
        }
        return max.value;
    }
};

void printTimestamp(std::chrono::high_resolution_clock::time_point timestamp, FILE* file) {
    auto duration = std::chrono::duration_cast<std::chrono::seconds>(timestamp.time_since_epoch());
    auto hours = std::chrono::duration_cast<std::chrono::hours>(duration).count() % 24;
    duration -= std::chrono::hours(hours);
    auto minutes = std::chrono::duration_cast<std::chrono::minutes>(duration).count() % 60;
    duration -= std::chrono::minutes(minutes);
    auto seconds = duration.count() % 60;

    fprintf(file, "%02ld:%02ld:%02ld", hours, minutes, seconds);
}

//const int k = 100; // Some constant value
const double p = 0.5; 

void testAtomic(int id, AtomicMRMWRegister<int>& shVar, int numOps) {
    FILE* file = fopen("logfile.txt", "a"); 
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
            localVar = shVar.read();
            fprintf(file, "Value read: %d\n", localVar);
        } else {
            localVar = numOps * id;
            shVar.write(localVar, id);
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

int capacity ;
int numOps ;
double Lambda ;

int main(){
    std:: ifstream inFile("inp-params.txt",std::ifstream::in);
    if(inFile.fail()) {
		printf("File not found");
		exit(1);
	}
    inFile >> capacity >> numOps >> Lambda ;
    int initialValue=0 ;
    AtomicMRMWRegister<int> shVar(capacity, initialValue);

    vector<thread>threads ;
    
    auto start = chrono::high_resolution_clock::now();

    for (int i = 0; i < capacity; ++i) {
        threads.emplace_back([i, &shVar]() { testAtomic(i, shVar, numOps); });
    }

    for (auto& thread : threads) {
        thread.join();
    }

    auto stop = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds>(stop - start);

        cout << "Time taken: " << duration.count() << " milliseconds" << endl;

    return 0 ;
}
