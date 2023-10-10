/*

NAME-SOUVIK SARKAR
ROLL-CS23MTECH02001

*/
#include <iostream>
#include <fstream>
#include <random>
#include <thread>
#include <unistd.h>
#include "includes/BOCC.h"

using namespace std ;

int n, m ,numTrans , constVal ,lambda ;
std::default_random_engine generator;
std::exponential_distribution<double>exp_dist(lambda) ;

std::ofstream outFile;
std::string fileName;

void update_mem(BOCC * bocc, int numTrans, int tid) ;


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

    BOCC* bocc= new BOCC(m) ;

    thread t_id[n] ;



    for(int i=0 ; i< n ; i++){
        t_id[i]=thread(&update_mem,bocc,n,i) ;
    }
    for(int i=0 ; i< n ; i++){
        t_id->join() ;
    }

}

void update_mem(BOCC * bocc, int numTrans, int tid){
    string status="abort" ;
    int abortCnt;
    chrono::high_resolution_clock::time_point critStartTime;   
    chrono::high_resolution_clock::time_point critEndTime;

    outFile.open("BOCC_log_file.txt",std::ios::out) ;

    for(int curTrans=0 ; curTrans<numTrans ; curTrans++){
        abortCnt=0 ;
        critEndTime=chrono::high_resolution_clock::now();
        do{
            int id= bocc->BOCC::begin_trans() ;
            int randIters= rand()%m+1 ;
            int locVal ;
            for(int i=0 ;i< randIters ;i++){
                int randInd= rand()%m ;
                int randVal= rand()%constVal ;
                bocc->read(id,randInd,&locVal) ;
                auto g1 = std::chrono:: high_resolution_clock::now();
                std::time_t now1 = std::chrono::system_clock::to_time_t(g1);
                outFile<<"Thread Id "<< tid<<"Transaction Id "<< tid<<"reads from "<<randInd<<"a value "<<locVal<<"at time "<<ctime(&now1);
                 
            
            locVal +=randVal ;
            bocc->write(id,randVal,locVal) ;

            auto g2 = std::chrono:: high_resolution_clock::now();
            std::time_t now2 = std::chrono::system_clock::to_time_t(g2);
            outFile<<"Thread Id "<< tid<<"Transaction Id "<< tid<<"reads from "<<randInd<<"a value "<<locVal<<"at time "<<ctime(&now2);
                 
        
            usleep(1000*exp_dist(generator)) ;
            }
            
           if(status=="commit") abortCnt++;
        }while(status!="commit");
    }
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(critEndTime-critStartTime).count();

    //no_aborts[tid]+=abortCnt ;
}