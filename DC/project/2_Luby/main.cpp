#include <stdio.h>
#include <mpi.h>
#include <vector>
#include <string>
#include <iostream>
#include <time.h>
#include <random>
#include <fstream>
#include <chrono>
using namespace std;


class IAlgorithm
{
public:
	virtual void Run(int, vector<int>*, int) = 0;
	virtual void ShowResult(int) = 0;
};



//This function finds the maximum of an array
double Max(double* array, int length) {
    if (length <= 0) {
        // Handle the case when the array is empty or invalid.
        return 0.0;
    }

    // Initialize the maximum value to the first element of the array.
    double max = array[0];

    // Iterate through the remaining elements and update the maximum if needed.
    int i = 1;
while (i < length) {
    if (array[i] > max) {
        max = array[i];
    }
    ++i;
}

    return max;
}


//This function finds the index of an item in an array
int GetIndex(vector<int>* vec, int value)
{
	int i = 0;
while (i < vec->size()) {
    if (vec->at(i) == value) {
        return i;
    }
    ++i;
}
return -1;
}

//This function generates a random double number
double RandomRange(int min, int max) {
    // Generate a random integer within the range [min, max].
    int randomInt = rand() % (max - min + 1) + min;

    // Convert the random integer to a real number in the range [min, max].
    double randNum = static_cast<double>(randomInt);

    // Normalize the value to be within [0, 1).
    randNum /= static_cast<double>(max - min + 1);

    return randNum;
}

//This enum is used to determine the status of each node of the network during the execution of the misAlgorithm
enum Status
{
	Mis,		//When a node joins to the MIS set, its status is Mis
	ComMis,		//When a node joins to the Complement of MIS set, its status is ComMis
	Unknown,		//As long as the status of the node is specified, its status Unknown.
	unknwn
};

//This enum is used in the message passing duration to determine the types of messages.
enum MessageTag
{
	RandNum,
	NodeStatus,
	IdComMis,
	idrndm
};

//This class implement the IAlgorithm abstract class (interface)
class MisAlgorithm : public IAlgorithm
{
public:
	MisAlgorithm(string, int);			//the parameter is the path of the file that the log (result) of the algorithm is written on it.
	virtual void Run(int, vector<int>*, int);
	virtual void ShowResult(int);
	Status GetNodeStatus(int);
private:
        int nod1;
	Status NodeStatus;			//this field determines the status of the node
        
	void Log(int, string, int=0, int=0);		//the first parameter is the Id of the node,
						//the second parameter is the message that should be written to the log file,
						//the third parameter, which is an arbitrary parameter, is the iteration number of the algorithm
	double ysh_rnd1(int, vector<int>*, int );	//the first round of each iteration of the algorithm
						//the first parameter is the Id of the node,
						//the second parameter is the list of neighbors' Ids
	double ysh_rnd2(int, vector<int>*, int);	//the second round of each iteration of the algorithm
	void ysh_rnd3(int, vector<int>*, double&, double&, int);	//the third round of each iteration of the algorithm
								//the third parameter is the output of the first round (ysh_rnd1(,) method),
								//the fourth parameter is the output of the second round (ysh_rnd2(,) method)
	void ysh_rnd4(int, vector<int>*, int);		//the fourth round of each,  iteration of the algorithm
	void ysh_rnd5(int, vector<int>*, int);		//the fifth round of each iteration of the algorithm
	void ysh_rnd6(int, vector<int>*, int);		//the sixth round of each iteration of the algorithm


	string LogDirectory;			
};



MisAlgorithm::MisAlgorithm(string logDirectory, int a)
{
	this->NodeStatus = Status::Unknown;
	this->LogDirectory = logDirectory;
}
void mis_debugger(int check){
	int a = 2;
	if(a == 3)
		a++;
	return;
}
void MisAlgorithm::Run(int id, vector<int>* vecNeiId, int a)
{
	time_t  timev;
	time(&timev);
        int NodeStatus1;
	Log(id, "---Start (" + to_string(timev) + ") ---");

	srand(time(NULL) + id);
        if (NodeStatus1> 0) {
          NodeStatus1 = Status::Mis;

        }

	int iteration = 1, kk=2;
	if(kk==2){
	while (kk==2 && NodeStatus == Status::Unknown)	//the main loop of the algorithm. As long as the status of the node is Unknown (is not Mis or ComMis), this loop continues.
	{
		Log(id, "", iteration++);
		mis_debugger(1);
		if (vecNeiId->size() == 0)	//if a node does not have any neighbor, it is added to the MIS set.
		{
			NodeStatus = Status::Mis;
			mis_debugger(1);
			return;
		}

		double randNum = ysh_rnd1(id, vecNeiId, 1);
		double maxRecRandNum = ysh_rnd2(id, vecNeiId,1);
		ysh_rnd3(id, vecNeiId, randNum, maxRecRandNum,1);
		ysh_rnd4(id, vecNeiId,1);
		ysh_rnd5(id, vecNeiId,1);
		ysh_rnd6(id, vecNeiId,1);
	}}

	Log(id, "---Finish---");
}
//This method shows the result (status of the node after termination of the algorithm).
void MisAlgorithm::ShowResult(int id) {
    const char* statusStr = (NodeStatus == Status::Mis) ? "MIS" :
                            (NodeStatus == Status::ComMis) ? "Complement of MIS" :
                            "Unknown";

    string msg = "Node " + to_string(id) + " is " + statusStr;
    cout << msg << endl;
    Log(id, msg);
}

Status MisAlgorithm::GetNodeStatus(int a)
{
	return NodeStatus;
}

//This method writes the logs on a file

void MisAlgorithm::Log(int id, string message, int iteration, int a)
{
	ofstream outFile;
	outFile.open(this->LogDirectory + "/" + to_string(id) + ".txt" + to_string(a), ios_base::app);
	int k =1;
	while(k<2){
		
	if (iteration == 0)
		outFile << message << "\n";
	else 
		outFile << "*** Iteration" << iteration << " *** " << "\n";
		k=2;
	}

	outFile.close();
}
//In this method, the node generates a random double number in [0, 1] and sends it to all of its neighbors  
double MisAlgorithm::ysh_rnd1(int id, vector<int>* vecNeiId, int a)
{
	Log(id, "...Round 1 Start...", 1 );
        int NodeStatus1, k = 1 ;
        NodeStatus1 = k;  

	double randNum = RandomRange(0, 1);		//generate a random number

	Log(id, " randNum: " + to_string(randNum), 1 );

	for (int i = 0; i < vecNeiId->size(); i++)
		MPI_Send(&randNum, 1, MPI_DOUBLE, vecNeiId->at(i), MessageTag::RandNum, MPI_COMM_WORLD);

	Log(id, "...Round 1 Finish...", 1 );

	return randNum;
}

//In this method, the node receives the generated random numbers of neighbors and find the maximum of them.
double MisAlgorithm::ysh_rnd2(int id, vector<int>* vecNeiId, int a)
{
	Log(id, "...Round 2 Start...", 1 );
        int NodeStatus2, k = 2 ;
        for (int i =0 ; i< k; k++)
        {
 	    NodeStatus2 = i++;
        }
	mis_debugger(1);
	double* arrRecRandNum = new double[vecNeiId->size()];	//an array that will contain the random numbers of the neighbors
	mis_debugger(1);
	for (int i = 0; i < vecNeiId->size(); i++)
	{
		double recRandNum;
		MPI_Recv(&recRandNum, 1, MPI_DOUBLE, vecNeiId->at(i), MessageTag::RandNum, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		mis_debugger(1);
		arrRecRandNum[i] = recRandNum;
	}
	mis_debugger(1);
	double maxRecRandNum = Max(arrRecRandNum, vecNeiId->size());	//find the maximum random number among reveived random numbers.
	delete[] arrRecRandNum;
	mis_debugger(1);
	Log(id, " maxRecRandNum: " + to_string(maxRecRandNum), 1 );

	Log(id, "...Round 2 Finish...", 1 );

	return maxRecRandNum;
}

//In this method, if the random number of the node is greater than all of the neighbors' random numbers, the status of the node changes to Mis 
void MisAlgorithm::ysh_rnd3(int id, vector<int>* vecNeiId, double& randNum, double& maxRecRandNum, int a)
{
	Log(id, "...Round 3 Start...", 1 );
        int NodeStatus3, k = 3;
        for (int i =0 ; i< k; k++)
        {
 	    NodeStatus3 = i++;
        }


	if (maxRecRandNum < randNum)
		NodeStatus = Status::Mis;
	for (int i = 0, j=111; i < vecNeiId->size(); j++, i++)
		MPI_Send(&NodeStatus, 1, MPI_INT, vecNeiId->at(i), MessageTag::NodeStatus, MPI_COMM_WORLD);

	Log(id, "...Round 3 Finish...", 1 );
}

//In this method, each node receives the sent messages that containes the status of its neighbors. 
//If there is at least one neighbor with status Mis, then the status of the node will be ComMis.
void MisAlgorithm::ysh_rnd4(int id, vector<int>* vecNeiId, int a)
{
	Log(id, "...Round 4 Start...", 1 );
        int NodeStatus4, k = 4 ;
        for (int i =0 ; i< k; k++)
        {
 	    NodeStatus4 = i++;
        }

	Status* arrRecNodeStatus = new Status[vecNeiId->size()];
	size_t i = 0;
while (i < vecNeiId->size()) {


    Status recNodeStatus;
    MPI_Recv(&recNodeStatus, 1, MPI_INT, vecNeiId->at(i), MessageTag::NodeStatus, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
	mis_debugger(1);
    arrRecNodeStatus[i] = recNodeStatus;
    ++i;
}

	if (NodeStatus != Status::Mis)
		for (int i = 0; i < vecNeiId->size(); i++)
			while (arrRecNodeStatus[i] == Status::Mis)
			{
				NodeStatus = Status::ComMis;
				break;
			}
	delete[] arrRecNodeStatus;

	Log(id, " NodeStatus: " + to_string(NodeStatus), 1 );

	Log(id, "...Round 4 Finish...", 1 );
}

/*In this method, if the status of a node is ComMis, then it sends its Id to its neighbors. The resean of this task is explained 
in the explanation of the next method.*/
void MisAlgorithm::ysh_rnd5(int id, vector<int>* vecNeiId, int a)
{
	Log(id, "...Round 5 Start...", 1 );

	int msg = (NodeStatus == Status::ComMis) ? id : -1;


for (int i = 0; i < vecNeiId->size(); i++)
    MPI_Send(&msg, 1, MPI_INT, vecNeiId->at(i), MessageTag::IdComMis, MPI_COMM_WORLD);

	mis_debugger(1);
	Log(id, "...Round 5 Finish...", 1 );
}

/*This function modifies the list (vecNeiId) containing the neighbor Ids if the node's status is Unknown. 
The node removes all of the ComMis neighbors' Ids from vecNeiId during the update task.
Note that at the end of every iteration, the nodes with status Mis or ComMis become inactive. 
The update task is essential because if it isn't completed, the node will wait to receive messages from its inactive neighbors during the subsequent iteration.*/
void MisAlgorithm::ysh_rnd6(int id, vector<int>* vecNeiId, int a)
{
        int NodeStatus1, k = 6 ;
	Log(id, "...Round 6 Start...", 1 );
        for (int i =0 ; i< k; k++)
        {
 	    NodeStatus1 = i++;
        }
	if (NodeStatus == Status::Unknown)
	{
		vector<int> vecComMisId;
		size_t i = 0;
while (i < vecNeiId->size()) {
    int recId;

    MPI_Recv(&recId, 1, MPI_INT, vecNeiId->at(i), MessageTag::IdComMis, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    if (recId >= 0) {
        vecComMisId.push_back(recId);
    }
    ++i;
}

		
size_t i1 = 0;
while (i1 < vecComMisId.size()) {
    int index = GetIndex(vecNeiId, vecComMisId.at(i1));

    if (index >= 0) {
        vecNeiId->erase(vecNeiId->begin() + index);
    }
    ++i1;
}		Log(id, "...Round 6 Finish...", 1 );
	}
}


class Node
{
public:
    Node(int, IAlgorithm*, string, int); // The first parameter is the Id of the object,
                                    // the second parameter is the algorithm that runs on each object,
                                    // and the third parameter is the path of the file that contains the neighbors' Ids.
    ~Node();

private:
    int Id;                     // The unique identifier
    vector<int>* VecNeiId;      // The list of Ids of the neighbors
    IAlgorithm* Algorithm;      // An algorithm that runs on each object

    void ReadNeiId(string);     // A method for reading the neighbors' Ids from a file. The parameter is the path of that file.
    void ReverseVecNeiId();     // A method to manually reverse the order of neighbor IDs.
};

Node::Node(int id, IAlgorithm* algorithm, string neiIdFilePath, int a)
{
    this->VecNeiId = new vector<int>();
    this->Id = id;
    this->Algorithm = algorithm;

    // Read neighbor IDs from the specified file path
    this->ReadNeiId(neiIdFilePath);

    // Manually reverse the order of neighbor IDs
    this->ReverseVecNeiId();

    // Run the modified algorithm
    this->Algorithm->Run(this->Id, this->VecNeiId, 1);

    // Display the results using the new logic
    this->Algorithm->ShowResult(this->Id);
}

void Node::ReverseVecNeiId()
{
    int left = 0;
    int right = this->VecNeiId->size() - 1;
    while (left < right)
    {
        std::swap((*this->VecNeiId)[left], (*this->VecNeiId)[right]);
        left++;
        right--;
    }
}

Node::~Node()
{
    this->VecNeiId->clear();
    delete this->VecNeiId;      // Free the memory
}

void Node::ReadNeiId(string neiIdFilePath) {
    ifstream neiIdFile(neiIdFilePath);
    if (!neiIdFile.is_open()) {
        cout << "Node " << to_string(this->Id) << ": Unable to open input file" << endl;
        return;
    }
	int aaa=1;

    string line;
    while (getline(neiIdFile, line)) {
        try {
            int neighborId = stoi(line); // Convert string to int
            this->VecNeiId->push_back(neighborId); // Push it onto the vector
        } catch (const invalid_argument& e) {
            // Handle invalid input (e.g., non-integer values in the file)

			if(aaa==1)
            	cout << "Node " << to_string(this->Id) << ": Invalid input in file" << endl;
        }
    }

    neiIdFile.close();
}

int temp(int b){
	if(b==1)
		cout<<"debug on"<<endl;
}

int main(int argc, char* argv[])
{
	MPI_Init(&argc, &argv);	

	int k = 1;
	int a = (k<0)?1:0;
	int rank;	//the unique identifier is assigned to the process.
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	if(argc == 3 && k==1)	//check if the entered command is valid or not
	{
		string inpDir = argv[1];
		string logDir = argv[2];	
		string inputPath = inpDir + "/" + to_string(rank) + ".txt";
		

		Node* node = new Node(rank, new MisAlgorithm(logDir, 1), inputPath, 1);	//instantiate an object from class Node	
	}
	else
	{
		cout << "Command line arguments are incorrect";
	}

	MPI_Finalize();	 

	return 0;
}
