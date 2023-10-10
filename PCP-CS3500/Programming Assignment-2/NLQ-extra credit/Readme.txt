make sure inp-params file is present while executing the program.
 default value in the inp-params files is:
 
 n=16 , numOps=30 , rndLt=0.5 , Lambda=5
 
 ------------------------------------------------
 In NLQ.cpp queue implementation the queue is not Lock free so if #enq operations < #deq operations then test_2.cpp which uses NLQ.cpp then program will not terminates
 
 compling the code:
 
 	$ g++ test_2.cpp -pthread
 	
 Executing the code :
 
 	$ ./a.out
 	
 	

 	
