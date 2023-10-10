make sure inp-params file is present while executing the program.
 default value in the inp-params files is:
 
 n=16 , numOps=30 , rndLt=0.5 , Lambda=5
 
 	
------------------------------------------------
 In NLQ_deterministic.cpp queue implementation the queue is Lock free, because in this we forcefully free the deq thread even though # enq Operation < #deq Operation. so this program will definatly terminates.
 
 compling the code:
 
 	$ g++ test_3.cpp -pthread
 	
 Executing the code :
 
 	$ ./a.out
 	
