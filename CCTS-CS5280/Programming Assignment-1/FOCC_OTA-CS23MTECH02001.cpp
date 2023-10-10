/*

NAME-SOUVIK SARKAR
ROLL-CS23MTECH02001

*/
#include<iostream>
#include<mutex>
#include<set>
#include<vector>
#include<string>
#include<chrono>
#include<atomic>
#include<fstream>
#include<thread>
#include<random>
#include<unistd.h>
#include<unordered_map>

using namespace std ;

class Transaction{
    private:

    string trans_status ;     //transaction status Ex:"commit","abort"&"live"
    int no_dataitems ;      //number of dataitems accessed by the transaction 

    public:
    int transaction_Id ;   //transaction ID
    set<int>readset ;       //Dataitems read by the transaction
    set<int>writeset ;      //Dataitems write by the transaction
    std::chrono::high_resolution_clock::time_point Start;   //transaction Begin time
    std::chrono::high_resolution_clock::time_point End;     //transaction Commit or Abort time
    vector<int>LocalBuffer ;//Local data buffer
    mutex trans_status_mtx ;        //Lock for transaction status update

    Transaction(int i , int x) ;
    string getStatus() ;
    void setStatus(std::string a) ;
    int read(int data) ;
    int write(int data, int value) ;
    
};

Transaction::Transaction(int i, int x){
    this->transaction_Id=i ;
    this->no_dataitems=x ;
    this->trans_status="live" ;
    while(no_dataitems-->0){
        LocalBuffer.push_back(-1) ;
    }
}

string Transaction::getStatus(){
    return this->trans_status ; ;
}

void Transaction::setStatus(string a){
    this->trans_status=a ;
}

int Transaction::read(int data){
    readset.insert(data) ;
    return 0 ;
}

int Transaction::write(int data, int value){
    LocalBuffer[data]=value ;
    writeset.insert(data) ;
    return 0 ;
}


class Dataitem{

    public:

    int dataitem_id ;
    int vaule ;
    mutex datalock ;
    set<Transaction *>readlist ;    //keeps the transaction info which read it
    set<Transaction *>writelist ;   //keeps the transaction info which wrote it
    Dataitem(int v) ;
};

Dataitem::Dataitem(int v){
    this->vaule=v ;
}

class FOCC_OTA{
    private :

    int no_dataitems ;
    mutex cnt;
    vector<Dataitem*> dataitem ;
    unordered_map<int,Transaction*> transaction ;
    set<int> conflict_transaction;
    void checkConflicts(const Transaction& tx);

    public:

    FOCC_OTA(int N) ;
    int begin_trans() ;
    string abortTransaction(int tid) ;
    string commitTransaction(int tid) ;
    int read(int tid, int index, int * buffer ) ;
    int write(int tid, int index, int value) ;
    string tryC(int tid) ;
};


std::atomic<int> counter(0);            //atomic counter for giving transaction id

FOCC_OTA::FOCC_OTA(int N){                      //Constructor
    this->no_dataitems=N ;
}

int FOCC_OTA::begin_trans(){
    int id=counter ++ ;
    transaction[id]=new Transaction(id,no_dataitems) ;                  //creating transaction object
    transaction[id]->Start=std::chrono::high_resolution_clock::now();   //starting time stam of the transaction
    return id ;     //here we are returning transaction id instead of transaction pointer because we are maintaing a map (id-> transaction pointer)
}

int FOCC_OTA::read(int tid, int index, int * buffer){
    // Lock the data item and transaction status mutexes
   
    dataitem[index]->datalock.lock() ;
    Transaction* current_trans ;
    current_trans= transaction[tid] ;
    current_trans->trans_status_mtx.lock() ;

    //If some other transaction abort this transaction dure to a conflict then erase it from the Writelist
    if(current_trans->getStatus()=="abort"){
        dataitem[index]->writelist.erase(current_trans) ;
    }
    else{
    //Read the value and update the transaction
    Dataitem * current_data;
    current_data= dataitem[index] ;
    * buffer=current_data->vaule ;
    current_trans->read(index) ;
    }
    // Unlock the mutexes
    dataitem[index]->datalock.unlock() ;
    current_trans->trans_status_mtx.unlock() ;

    return 0 ;
}

//"Write" Fuction basically perform write operation on it's local buffer

int FOCC_OTA::write(int tid, int index, int value){
    transaction[tid]->write(index,value) ;                  //Perform the write operation in the transaction's local buffer
    dataitem[index]->datalock.lock() ;                      //Acquire the lock on the data item's mutex

    dataitem[index]->writelist.insert(transaction[tid]) ;   //Add the current transaction to the write list of the data item

    dataitem[index]->datalock.unlock() ;                    //Release the lock on the data item's mutex
    return 0 ;
}

int n, m ,numTrans , constVal ,lambda ;
std::default_random_engine generator;
std::exponential_distribution<double>exp_dist(lambda) ;

std::ofstream outFile;
std::string fileName;

void update_mem(FOCC_OTA * focc, int numTrans, int tid) ;

int* no_abort = new int[n];
int* delay = new int[n];


int main(){
    ifstream inputfile("inp-params.txt");
    if(!inputfile.is_open()){
        cout<<"Error while opening the input file" ;
        return 0 ;
    }
    inputfile>> n ;
    inputfile>> m ;
    inputfile>> numTrans ;
    inputfile>> constVal ;
    inputfile>> lambda ;

    FOCC_OTA* focc= new FOCC_OTA(m) ;

    thread t_id[n] ;



    for(int i=0 ; i< n ; i++){
        t_id[i]=thread(&update_mem,focc,n,i) ;
    }
    for(int i=0 ; i< n ; i++){
        t_id->join() ;
    }

}

void update_mem(FOCC_OTA * focc, int numTrans, int tid){
    string status="abort" ;
    int abortCnt;
    chrono::high_resolution_clock::time_point critStartTime;   
    chrono::high_resolution_clock::time_point critEndTime;

    outFile.open("FOCC_OTA_log_file.txt",std::ios::out) ;

    for(int curTrans=0 ; curTrans<numTrans ; curTrans++){
        abortCnt=0 ;
        critEndTime=chrono::high_resolution_clock::now();
        do{
            int id= focc->FOCC_OTA::begin_trans() ;
            int randIters= rand()%m+1 ;
            int locVal ;
            for(int i=0 ;i< randIters ;i++){
                int randInd= rand()%m ;
                int randVal= rand()%constVal ;
                focc->read(id,randInd,&locVal) ;
                auto g1 = std::chrono:: high_resolution_clock::now();
                std::time_t now1 = std::chrono::system_clock::to_time_t(g1);
                outFile<<"Thread Id "<< tid<<"Transaction Id "<< tid<<"reads from "<<randInd<<"a value "<<locVal<<"at time "<<ctime(&now1);
                 
            
            locVal +=randVal ;
            focc->write(id,randVal,locVal) ;

            auto g2 = std::chrono:: high_resolution_clock::now();
            std::time_t now2 = std::chrono::system_clock::to_time_t(g2);
            outFile<<"Thread Id "<< tid<<"Transaction Id "<< tid<<"reads from "<<randInd<<"a value "<<locVal<<"at time "<<ctime(&now2);
                 
        
            usleep(1000*exp_dist(generator)) ;
            }
            
           if(status=="commit") abortCnt++;
        }while(status!="commit");
    }
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(critEndTime-critStartTime).count();
    delay[tid]=duration ;
    no_abort[tid]+=abortCnt ;

}