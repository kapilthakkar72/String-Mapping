#include <stdio.h>
#include <string>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <map>
#include <vector>
#include <algorithm>
#include <fstream>
#include <queue>
#include <limits>
#include <float.h>
#include <signal.h>
#include <unistd.h>

using namespace std;

#define MAX_ARRAY_SIZE 150

struct Node
{
    long cost;
    int parent_cost;
    map<int, string> setOfStringsInMap;
    Node *parent;
};

class CompareNodes
{
    public:

    bool operator() (const Node comp1, const Node comp2) const
    {
        return ((comp1.cost) > (comp2.cost));
    }
};

priority_queue<Node, vector<Node>, CompareNodes> nodeQueue;

Node optimal;

int noOfStrings = -1;
int costOfInsertingSpace = -1;
int costWithSpace = -1;
int costWithCharacter = -1;
double timeLimit = -1;
int noOfChars = -1;
vector<string> setOfStrings;
vector<string> characterSet;
vector<string> MCMatrix;
map <string, int> costs;
time_t start;
int randomNumberArray[MAX_ARRAY_SIZE];

int getCost (char a, char b);

void splitString (string message, string delimiter, string result[])
{
    int i = 0;
    int pos = 0;
    int length = 0;
    int temp;
    temp = message.find (delimiter.c_str (), pos);
    while (temp != -1)
    {
        length = temp - pos;
        //printf ("pos : %d length : %d\n",pos,length);
        result[i] = message.substr (pos, length);
        pos = temp + delimiter.size ();
        temp = message.find (delimiter.c_str (), pos);
        i++;
    }

    result[i] = message.substr (pos);
    i++;

}

void createMapOfMC ()
{
    // take out one by one column from the vector and create map 

    string splittedRow[noOfChars + 1];
    int i, j, temp;
    for (i = 0; i < noOfChars + 1; i++)
    {
        splitString (MCMatrix.at (i), " ", splittedRow);
        for (j = 0; j < noOfChars + 1; j++)
        {
            temp = atoi (splittedRow[j].c_str ());

            /* temp stores the cost in between character i & j */
            string a (characterSet.at (i));
            string b (characterSet.at (j));
            string c = a + b; // to be put into map

            costs[c] = temp;
        }
    }
}

int readInputFile (string inputFileName)
{
    string line;
    ifstream inputFile;
    inputFile.open (inputFileName.c_str ());

    int count = 1; // To count the current line in process
    int stringCounter = 0; // will be used while storing strings to vector
    int matrixRows = 0; // will be used to create and store MC
    if (inputFile.is_open ())
    {
        while (getline (inputFile, line))
        {
            /* "line" contains the line read */

            if (count == 1)
            {
                /* First line contains the time limit*/
                timeLimit = atof (line.c_str ());
                count++;
            }
            else if (count == 2)
            {
                /* Second line contains number of characters*/
                noOfChars = atoi (line.c_str ());
                count++;
            }
            else if (count == 3)
            {
                /* Third line contains set of characters */
                string splitCharacters[noOfChars + 1];
                splitString (line, ", ", splitCharacters);
                for (int i = 0; i < noOfChars; i++)
                {
                    characterSet.push_back (splitCharacters[i]);
                }
                characterSet.push_back ("-");
                count++;
            }
            else if (count == 4)
            {
                /* Fourth Line contains number of strings */
                noOfStrings = atoi (line.c_str ());
                count++;
            }
            else if (line.compare ("#") == 0)
            {
                break;
            }
            else if (stringCounter < noOfStrings)
            {
                /* It contains one of the strings */
                setOfStrings.push_back (line);
                stringCounter++;
            }
            else if (stringCounter == noOfStrings)
            {
                /* CC cost*/
                costOfInsertingSpace = atoi (line.c_str ());
                stringCounter++;
            }
            else
            {
                /* MC cost */
                /* Create Matrix first, then we will put costs into map */

                MCMatrix.push_back (line);

                matrixRows++;

                if (matrixRows == noOfChars + 1)
                {
                    createMapOfMC ();
                }

            }

        }
        inputFile.close ();
        return 1;
    }
    else cout << "Unable to open file";
    return 0;
}

int getCost (char a, char b)
{
    string none = "";
    string as = none + a;
    string bs = none + b;
    string cs = as + bs;

    map<string, int>::iterator it = costs.find (cs);

    if (it != costs.end ())
    {
        return costs[cs];
    }
    else
    {
        return -1;
    }
}

/*
 Directions:
 * 1- Insert
 * 2- Delete
 * 3- Copy
 * 4- Replace
 */

string moves (int directions[MAX_ARRAY_SIZE][MAX_ARRAY_SIZE], int a, int b)
{
    // TODO Auto-generated method stub

    if (a == 0 && b == 0)
    {
        return "";
    }

    //1--left;2--up;3--copy;4--Replace
    if (directions[a][b] == 1)
    {
        return moves (directions, a, b - 1) + "1";
    }
    else if (directions[a][b] == 2)
    {
        return moves (directions, a - 1, b) + "2";
    }
    else if (directions[a][b] == 3)
    {
        return moves (directions, a - 1, b - 1) + "3";
    }
    else
    {
        return moves (directions, a - 1, b - 1) + "4";
    }
}

string handleEditingInNewEditDistance(string s1,string s[],int n, string pattern, string result[])
{
    string newS[n];
    string s1New="";

    for (int i = 0; i < n; ++i)
    {
        newS[i]="";
    }

    /******* we need to change both s1 and s[] simulteneously **********/\
    int j=0; //for s1
    int k=0; //for s[]

    for (int i = 0; i < pattern.length (); i++)
    {
        if (pattern.at (i) == '1')
        {
            s1New+="-";

            for (int p = 0; p < n; ++p)
            {
                newS[p]+=s[p].at(k);
            }
            k++;
        }
        else if(pattern.at(i)=='2')
        {
            s1New+=s1.at(j);
            j++;
            for (int p = 0; p < n; ++p)
            {
                newS[p]+="-";
            }
        }
        else
        {
            s1New+=s1.at(j);
            j++;
            for (int p = 0; p < n; ++p)
            {
                newS[p]+=s[p].at(k);
            }
            k++;
        }
    }
    
    for (int i = 0; i < n; ++i)
    {
        result[i]=newS[i];
    }
    return s1New;
}

void getStringsForThisNode(Node n,string s[])
{
    int i=0;
    for (map<int, string> ::iterator it = n.setOfStringsInMap.begin (); it != n.setOfStringsInMap.end (); it++){
        s[i]=it->second;
        i++;
    }

}

int getDifference(string a, string b)
{
    if(a.length ()!=b.length ())
    {
        cout<<"Error in getDifference\n";
        exit(0);
    }
    else
    {
        int cost=0;
        for(int i=0;i<a.length ();i++)
        {
            cost+=getCost (a.at (i),b.at(i));
        }
        return cost;
    }
}

int getSpaceCost(Node temp)
{
    int cost=0;
    for(map<int,string>::iterator it=temp.setOfStringsInMap.begin ();it!=temp.setOfStringsInMap.end ();it++){
        string str=it->second;
        for(int i=0;i<str.length();i++){
            if(str.at(i)=='-'){
                cost+=costOfInsertingSpace;
            }
        }
    }
    return cost;
}

int costOfInsert(int charNo, string s[],int n, char a)
{
   int cost=0;
   for (int i = 0; i < n; ++i)
   {
       cost+= getCost(a,s[i].at(charNo));
   }
   return cost;
}

int costOfDelete(int charNo, string s[],int n, char a)
{
   int cost=0;
   cost = n*costOfInsertingSpace;
   for (int i = 0; i < n; ++i)
   {
       cost+= getCost(a,'-');
   }
   return cost;
}

string modifiedEditDistance (string s1, string s2[], int * cost, int n)
{
    int len1 = s1.size ();
    int len2 = s2[0].size ();

    int editDist[MAX_ARRAY_SIZE][MAX_ARRAY_SIZE];
    int directions[MAX_ARRAY_SIZE][MAX_ARRAY_SIZE]; //1--insert;2--delete;3--copy;4--Replace

    editDist[0][0] = directions[0][0]= 0;

    // Initialise editDist with 0
    for (int i = 0; i <= len1; i++)
        for (int j = 0; j <= len2; j++)
        {
            directions[i][j]=0;
            editDist[i][j] = 0;
        }

   
     for (int i = 1; i <= len1; i++)
     {
        directions[i][0]=2;

        editDist[i][0] = editDist[i - 1][0] + costOfDelete(i-1,s2 ,n, s1.at(i-1)); //Delete Cost      

     }

    for (int i = 1; i <= len2; i++)
    {
        directions[0][i]=1;

        editDist[0][i] = editDist[0][i - 1] + costOfInsert(i-1,s2, n,'-') + costOfInsertingSpace; //Insert Cost
    }


    for (int i = 1; i <= len1; i++)
    {
        for (int j = 1; j <= len2; j++)
        {
            int min = 100000;

            /******* Changes *********/

            min = editDist[i][j - 1] + costOfInsertingSpace + costOfInsert(j-1,s2, n,'-');
            directions[i][j] = 1;

            
            if (min > (editDist[i - 1][j - 1] + costOfInsert(j-1,s2, n,s1.at(i-1))))
            {
                min = editDist[i - 1][j - 1] + costOfInsert(j-1,s2, n,s1.at(i-1));
                directions[i][j] = 3;
            }
            else if (min > (editDist[i-1][j] + costOfDelete (i-1,s2, n,s1.at(i-1))))
            {
                min = editDist[i - 1][j - 1] + costOfDelete(i-1,s2, n,'-');
                directions[i][j] = 2;
            }
            editDist[i][j] = min;
        }
    }

    *cost = editDist[len1][len2];
    
    return moves (directions, len1, len2);
}

int getMeCostOfThisNode(Node temp)
{
    int cost=0;
    for(map<int,string>::iterator it=temp.setOfStringsInMap.begin ();it!=temp.setOfStringsInMap.end ();it++)
    {
        it++;
        map<int,string>::iterator itInner=it;
        it--;
        for(;itInner!=temp.setOfStringsInMap.end () ;itInner++)
        {
            cost+=getDifference (it->second,itInner->second);
        }
    }

    cost+= getSpaceCost (temp);
	return cost;
}

// A utility function to swap to integers
void swap (int *a, int *b)
{
    int temp = *a;
    *a = *b;
    *b = temp;
}
 
// A utility function to print an array
void printArray (int arr[], int n)
{
    for (int i = 0; i < n; i++)
        printf("%d ", arr[i]);
    printf("\n");
}
 
// A function to generate a random permutation of arr[]
void randomize ( int arr[], int n )
{
    // Use a different seed value so that we don't get same
    // result each time we run this program
    srand ( time(NULL) );
 
    // Start from the last element and swap one by one. We don't
    // need to run for the first element that's why i > 0
    for (int i = n-1; i > 0; i--)
    {
        // Pick a random index from 0 to i
        int j = rand() % (i+1);
 
        // Swap arr[i] with the element at random index
        swap(&arr[i], &arr[j]);
    }
}

void getSetGo ()
{
    bool flag=false;
    int i, j;  

    while  (1)
    {
        // Generate Random sequence
		randomize(randomNumberArray,noOfStrings);

        Node temp;
        temp.setOfStringsInMap[randomNumberArray[0]] = setOfStrings.at(randomNumberArray[0]);
        temp.cost=0;

        nodeQueue.push(temp);

        flag = false;

        while (!nodeQueue.empty () && !flag)
        {
            // take out node from queue
            // Check it with all strings 

            for (i = 1; i < setOfStrings.size (); i++)
            {
                // check whether string i is processed in node.
                // If Not then process it with every string present in the node

                Node nodeInProcess = nodeQueue.top ();

                if (nodeInProcess.setOfStringsInMap.size () == noOfStrings)
                {
                    if(nodeInProcess.cost < optimal.cost)
                    {
                       
                        // // Optimal Node found
                        optimal.cost = nodeInProcess.cost;
                        for (map<int, string> ::iterator it = nodeInProcess.setOfStringsInMap.begin (); it != nodeInProcess.setOfStringsInMap.end (); it++)
                        {
                            optimal.setOfStringsInMap[it->first] = it->second;                            
                        }
                    }
                    nodeQueue.pop();
                    flag = true;
                    break;
                }

                map<int, string>::iterator it = nodeInProcess.setOfStringsInMap.find (randomNumberArray[i]);

                if (it == nodeInProcess.setOfStringsInMap.end ())
                {
                    // Generation of New Node considering all strings at once

                    int c=0,n=nodeInProcess.setOfStringsInMap.size();
                    string stringSet[n];
                    getStringsForThisNode(nodeInProcess,stringSet);
                    string stringInConsideration=setOfStrings.at(randomNumberArray[i]);
                    string pat=modifiedEditDistance(stringInConsideration,stringSet,&c,n);
                    Node temp;

					string stringSetResult[n];

                    for (int k = 0; k < n; ++k)
                    {
                        stringSetResult[k]="";
                    }

                    stringInConsideration = handleEditingInNewEditDistance(stringInConsideration,stringSet,n,pat,stringSetResult);

                    temp.setOfStringsInMap[randomNumberArray[i]]=stringInConsideration;

                    int j=0;
                    for (std::map<int, string>::iterator itInner = nodeInProcess.setOfStringsInMap.begin (); itInner != nodeInProcess.setOfStringsInMap.end (); ++itInner){
                        temp.setOfStringsInMap[itInner->first]=stringSetResult[j]; 
                        j++;          
                    }
                    temp.cost=getMeCostOfThisNode(temp);
                    nodeQueue.push(temp);
                }
                nodeQueue.pop();
            }       
        }
    }

}

void printOutput(int) {
   for(map<int,string>::iterator it=optimal.setOfStringsInMap.begin ();it!=optimal.setOfStringsInMap.end ();it++){
        cout<<it->second<<endl;
    }
    exit(0);
}

int main(int argc, char* argv[]) {
    if (argc != 2){
        cout<<"input output file not specified"<<endl;
        return -1;
    }
    signal(SIGALRM, printOutput);

      /* Check for validity of the input file and initialize variables if file is valid */
    string inputFle = argv[1];
    int i = readInputFile (inputFle);

    optimal.cost = 2147483647;

    for (int i = 0; i < MAX_ARRAY_SIZE; ++i)
    {
        randomNumberArray[i]=i;
    }

    int cost;
   
    alarm(timeLimit*60);
    getSetGo();

    return 0;
}