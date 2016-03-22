#include <stdio.h>
#include <string>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <map>
#include <vector>
#include <set>
#include <algorithm>
#include <fstream>
#include <queue>
#include <time.h>
#include <list>

using namespace std;

#define MAX_ARRAY_SIZE 50
#define PROCESSED_NODE_SIZE 500


struct Node
{
    int cost;
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
std::list<Node> procssedNodes;

Node optimal;
int maxCost=0;
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
int nodecount=0;
int getCost (char a, char b);
string editDistance (string s1, string s2[], int * cost, int n);
int getMeCostOfThisNode(Node temp);

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
            if(temp>maxCost)
                maxCost=temp;
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

string editDistance (string s1, string s2, int * cost)
{
    //cout<<"\nin editDistance call for "<<s1<<" "<<s2;
    int len1 = s1.size ();
    int len2 = s2.size ();
    int temp=0;

    int editDist[MAX_ARRAY_SIZE][MAX_ARRAY_SIZE];
    int directions[MAX_ARRAY_SIZE][MAX_ARRAY_SIZE]; //1--insert;2--delete;3--copy;4--Replace

    editDist[0][0] = directions[0][0] = 0;

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

        if (s1.at (i - 1) == '-')
        {
            editDist[i][0] = editDist[i - 1][0] + costOfInsertingSpace; //Delete Cost
        }
        else
        {
            editDist[i][0] = editDist[i - 1][0] + costOfInsertingSpace + getCost (s1.at (i - 1), '-'); //Delete Cost
        }
    }

    for (int i = 1; i <= len2; i++)
    {
        directions[0][i]=1;

        if (s2.at (i - 1) == '-')
        {
            editDist[0][i] = editDist[0][i - 1] + costOfInsertingSpace; //Insert Cost
        }
        else
        {
            editDist[0][i] = editDist[0][i - 1] + costOfInsertingSpace + getCost (s2.at (i - 1), '-'); //Insert Cost
        }
    }


    for (int i = 1; i <= len1; i++)
    {
        for (int j = 1; j <= len2; j++)
        {
            int min = 100000;
            /******* Changes *********/
            if (s2.at (j - 1) == '-')
            {
                min = editDist[i][j - 1] + costOfInsertingSpace;
            }
            else
            {
                min = editDist[i][j - 1] + costOfInsertingSpace + getCost (s2.at (j - 1), '-');
            }

            directions[i][j] = 1;

            if (s1.at (i - 1) == '-')
            {
                if (min > (editDist[i - 1][j] + costOfInsertingSpace))
                {
                    min = editDist[i - 1][j] + costOfInsertingSpace;
                    directions[i][j] = 2;
                }
            }
            else
            {
                if (min > (editDist[i - 1][j] + costOfInsertingSpace + getCost (s1.at (i - 1), '-')))
                {
                    min = editDist[i - 1][j] + costOfInsertingSpace + getCost (s1.at (i - 1), '-');
                    directions[i][j] = 2;
                }
                /*** this was just there previously  ****/
            }

            if (s1.at (i - 1) == s2.at (j - 1))
            {
                if (min > (editDist[i - 1][j - 1] + getCost (s1.at (i - 1), s2.at (j - 1))))
                {
                    min = editDist[i - 1][j - 1] + getCost (s1.at (i - 1), s2.at (j - 1));
                    directions[i][j] = 3;
                }
            }
            else
            {
                if (s1.at (i - 1) == '-')
                {
                    if (min > (editDist[i - 1][j - 1] + getCost (s2.at (j - 1), '-')))
                    {
                        min = editDist[i - 1][j - 1] + getCost (s2.at (j - 1), '-');
                        directions[i][j] = 4;
                    }
                }
                else if (s2.at (j - 1) == '-')
                {
                    if (min > (editDist[i - 1][j - 1] + getCost (s1.at (i - 1), '-')))
                    {
                        min = editDist[i - 1][j - 1] + getCost (s1.at (i - 1), '-');
                        directions[i][j] = 4;
                    }
                }
                else
                {
                    if (min > (editDist[i - 1][j - 1] + getCost (s1.at (i - 1), s2.at (j - 1))))
                    {
                        min = editDist[i - 1][j - 1] + getCost (s1.at (i - 1), s2.at (j - 1));
                        directions[i][j] = 4;
                    }
                }
            }
            editDist[i][j] = min;
        }
    }

    *cost = editDist[len1][len2];
    /* Function call to generate string of operations */
    return moves (directions, len1, len2);
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

void findAllLcs(string X,string Y,set<string> &lcsSet);
int c[MAX_ARRAY_SIZE][MAX_ARRAY_SIZE];

void LCS(string X, string Y,int &length,set<string> &lcsSet)
{
   int m,n;
     m = X.length(),n=Y.length();
    int i,j;
    for (i=0;i<=m;i++){
        for (j=0;j<=n;j++){
             c[i][j]=0;
        }
    }
    
    for (i=1;i<=m;i++)
         for (j=1;j<=n;j++)
         {
             if (X[i-1]==Y[j-1]){
                c[i][j]=c[i-1][j-1]+1;
            }
             else
             {
                if(c[i][j-1]>c[i-1][j]){
                    c[i][j]=c[i][j-1];
                }
                else{
                    c[i][j]=c[i-1][j];
                }
             }
         }
    length= c[m][n];
    findAllLcs(X,Y,lcsSet);
}

void findAllLcs(string X,string Y,set<string> &lcsSet){
    int l;
    int m=X.length(),n=Y.length();
    if (m == 0 || n == 0) {
        lcsSet.insert("");
    }
    else{
        if (X.at(m - 1) == Y.at(n - 1)) {
            set<string> set1;
            findAllLcs(X.substr(0,m-1), Y.substr(0,n-1),set1);
            for (set<string> ::iterator it=set1.begin (); it!=set1.end (); it++){
                string temp="";
                temp = *it + "3";
                lcsSet.insert(temp);
            }
        }
        else{
            set<string> set1;
            if (c[m - 1][n] > c[m][n - 1]){
                findAllLcs(X.substr(0,m-1), Y,set1);
                for (set<string> ::iterator it=set1.begin (); it!=set1.end (); it++){
                    string temp="";
                    temp = *it + "2";
                    lcsSet.insert(temp);
                }
            }
            if (c[m][n - 1] > c[m - 1][n]) {
                findAllLcs(X,Y.substr(0,n-1),set1);
                for (set<string> ::iterator it=set1.begin (); it!=set1.end (); it++){
                    string temp="";
                    temp = *it + "1";
                    lcsSet.insert(temp);
                }
            }
            if (c[m][n - 1] == c[m - 1][n]) {
                findAllLcs(X.substr(0,m-1), Y,set1);
                for (set<string> ::iterator it=set1.begin (); it!=set1.end (); it++){
                    string temp="";
                    temp = *it + "2";
                    lcsSet.insert(temp);
                }
                findAllLcs(X,Y.substr(0,n-1),set1);
                for (set<string> ::iterator it=set1.begin (); it!=set1.end (); it++){
                    string temp="";
                    temp = *it + "1";
                    lcsSet.insert(temp);
                }
            }
        }
    }
}


void findPosition(string pat,int p1[],int p2[],int length){
    int u=0;
    int k=0;
    int j=0;
    for(int i=0;i<pat.length();i++){
        if(pat.at(i)=='1')
            k++;
        else if(pat.at(i)=='2')
            j++;
        else{
            p2[u]=k;
            p1[u]=j;
            k++;j++;u++;
        }
    }
}

int validatePos(int aPos[],int bPos[],int n,int len1,int len2){
    for(int i=0;i<n;i++){
        if(aPos[i]<0||aPos[i]>len1-1)
            return 1;
        if(bPos[i]<0||bPos[i]>len2-1)
            return 1;
    }
    return 0;
}

void stringtoPos(string s,int aPos[],int bPos[]){
    int i;
    for(i=0;s.at(i)!=' ';i++){
        aPos[i]=s.at(i)-48;
    }
    for(int j=i+1;j<s.length();j++,i++){
        bPos[j]=s.at(i)-48;
    }
}
void skipRedundentNode(Node temp){
    int processThis = 1;
    int nodesMatched;

    for (std::list<Node>::iterator it=procssedNodes.begin(); it != procssedNodes.end(); ++it)
    {
        nodesMatched=0;
        processThis=1;

        if((*it).setOfStringsInMap.size() != temp.setOfStringsInMap.size())
        {
            continue;
        }
        else
        {
            
            for (map<int, string> ::iterator itInner = temp.setOfStringsInMap.begin (); itInner != temp.setOfStringsInMap.end (); itInner++)
            {
                if( (*it).setOfStringsInMap.find(itInner->first) == (*it).setOfStringsInMap.end() ) 
                {
                    break;
                }
                else
                {
                    if(((*it).setOfStringsInMap.find(itInner->first)->second).compare(temp.setOfStringsInMap.find(itInner->first)->second)==0 )
                    {
                        nodesMatched++;
                    }
                    else
                    {
                        break;
                    }               
                }
            }

            if(nodesMatched == (*it).setOfStringsInMap.size())
            {
                // node is processed
                processThis=0;
                break;
            }

        }
    }

    if(processThis == 0)
    {
        return;
    }
    procssedNodes.push_back(temp);
    nodeQueue.push(temp);
}

void generateLCSNode (string a, string b, int indexa, int indexb)
{
    int length;
    set<string> pat;
    LCS(a,b,length,pat);
    //cout<<"patterns: \n";
    int aPos[length],bPos[length];
    int c1=0,c2=0;
    set<string> lcsPosString;
    for (set<string>::iterator it = pat.begin (); it!= pat.end (); it++){
        findPosition(*it,aPos,bPos,length);
        if(validatePos(aPos,bPos,length,a.length(),b.length()))
            continue;
        string s1="",s2="";
        for(int i=0;i<length;i++){
            char arr[20];
            sprintf(arr,"%d",aPos[i]);
            s1+=string(arr);
            sprintf(arr,"%d",bPos[i]);
            s2+=string(arr);
        }
        c1++;
        s1+=" "+s2;
        if(lcsPosString.find(s1)==lcsPosString.end()){
            string newA="",newB="";
            int i=0;
            int k=0,j,l=0,m;
            while(length!=i){
                j=aPos[i];
                m=bPos[i];
                int c;
                if(j-k==0&&m-l==0){
                    newA+=a.at(j);
                    newB+=b.at(m);
                    k=j+1;
                    l=m+1;
                    i++;
                    continue;
                }
                string pattern=editDistance(a.substr(k,j-k),b.substr(l,m-l),&c);
                string s[1],stringSetResult[1];
                s[0]=b.substr(l,m-l);

                newA+=handleEditingInNewEditDistance(a.substr(k,j-k),s,1,pattern,stringSetResult);
                newB+=stringSetResult[0];

                newA+=a.at(j);
                newB+=b.at(m);

                k=j+1;
                l=m+1;
                i++;
            }

            int c;
            string pattern=editDistance(a.substr(k),b.substr(l),&c);
            string s[1],stringSetResult[1];
            s[0]=b.substr(l);

            newA+=handleEditingInNewEditDistance(a.substr(k),s,1,pattern,stringSetResult);
            newB+=stringSetResult[0];

            Node temp;
            temp.setOfStringsInMap[indexa]=newA;
            temp.setOfStringsInMap[indexb]=newB;
            temp.cost = getMeCostOfThisNode(temp);
            nodecount++;
            skipRedundentNode(temp);
            lcsPosString.insert(s1);
        }
    }
}


void generateFirstLevelOfNodes ()
{
    int i,j;

    for(j=0;j<noOfStrings;j++)
    {
        // Take out first string and create first set of children/nodes and put it into queue
        string primaryString = setOfStrings.at (j);

        for (i = 1; i < setOfStrings.size (); i++)
        {
            if(i!=j)
            {       
                Node temp;
                temp.setOfStringsInMap.clear();
                string secondaryString = setOfStrings.at (i);

                string pattern;
                int editDistanceCost;
                
                pattern = editDistance(primaryString,secondaryString,&editDistanceCost);

                string s[1];
                s[0] = secondaryString;

                string stringSetResult[1];

                temp.setOfStringsInMap[j] = handleEditingInNewEditDistance(primaryString,s,1,pattern,stringSetResult);
                temp.setOfStringsInMap[i] = stringSetResult[0];
                
                temp.cost = editDistanceCost;      
               
                nodecount++;
                skipRedundentNode(temp);
                if(maxCost>=costOfInsertingSpace)
                    generateLCSNode (setOfStrings.at(i),setOfStrings.at(j),i,j);
            }
        }
    }
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
    /* Function call to generate string of operations */
    
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

void getSetGo ()
{
    int i, j;

    generateFirstLevelOfNodes ();

    // Process Queue

    while (!nodeQueue.empty ())
    {
        // take out node from queue

        Node nodeInProcess = nodeQueue.top ();

        if (nodeInProcess.setOfStringsInMap.size () == noOfStrings)
        {
            // Optimal Node found
            optimal.cost = nodeInProcess.cost;
            for (map<int, string> ::iterator it = nodeInProcess.setOfStringsInMap.begin (); it != nodeInProcess.setOfStringsInMap.end (); it++)
            {
                optimal.setOfStringsInMap[it->first] = it->second;
            }
            break;
        }

        // Check it with all strings 

        for (i = 0; i < setOfStrings.size (); i++)
        {
            // check whether string i is processed in node.
            // If Not then process it with every string present in the node

            map<int, string>::iterator it = nodeInProcess.setOfStringsInMap.find (i);

            if (it == nodeInProcess.setOfStringsInMap.end ())
            {
            	// for each string of the node
            	for (map<int, string> ::iterator itNode = nodeInProcess.setOfStringsInMap.begin (); itNode != nodeInProcess.setOfStringsInMap.end (); itNode++)
	            {
	            	int cost;
	            	string primaryString = setOfStrings.at(i);
	                string pattern = editDistance(primaryString, itNode->second, &cost );

	                int n=nodeInProcess.setOfStringsInMap.size();
	                string s[n];
	               
	                string stringSetResult[n];

	                for (int k = 0; k < n; ++k)
	                {
	                	stringSetResult[k]="";
	                }

	                getStringsForThisNode(nodeInProcess,s);

	                string newAddition = handleEditingInNewEditDistance(primaryString, s, n, pattern,stringSetResult);

	                // add these nodes to temp node

	                Node temp;
	                temp.setOfStringsInMap[i] = newAddition;
	                j=0;
	                for (map<int, string> ::iterator itInner = nodeInProcess.setOfStringsInMap.begin (); itInner != nodeInProcess.setOfStringsInMap.end (); itInner++)
	            	{
	            		temp.setOfStringsInMap[itInner->first] = stringSetResult[j];
	            		j++;
	            	}

	            	temp.cost = getMeCostOfThisNode(temp);
                    nodecount++;
                    skipRedundentNode(temp);
	            }

	            // Generation of New Node considering all strings at once

	            int c=0,n=nodeInProcess.setOfStringsInMap.size();
                string stringSet[n];
                getStringsForThisNode(nodeInProcess,stringSet);
                string stringInConsideration=setOfStrings.at(i);
                string pat=modifiedEditDistance(stringInConsideration,stringSet,&c,n);
                Node temp;

	            string stringSetResult[n];

	            for (int k = 0; k < n; ++k)
	                {
	                	stringSetResult[k]="";
	                }

                stringInConsideration = handleEditingInNewEditDistance(stringInConsideration,stringSet,n,pat,stringSetResult);
                temp.setOfStringsInMap[i]=stringInConsideration;

                int j=0;
                for (std::map<int, string>::iterator itInner = nodeInProcess.setOfStringsInMap.begin (); itInner != nodeInProcess.setOfStringsInMap.end (); ++itInner){
                    temp.setOfStringsInMap[itInner->first]=stringSetResult[j]; 
                    j++;          
                }
                temp.cost=getMeCostOfThisNode(temp);
                nodecount++;
                skipRedundentNode(temp);
            }
        }

        nodeQueue.pop();
    }
}

int main(int argc, char* argv[]) {
    if (argc != 2){
        cout<<"input output file not specified"<<endl;
        return -1;
    }
      /* Check for validity of the input file and initialize variables if file is valid */
    string inputFle = argv[1];
    int i = readInputFile (inputFle);
    optimal.cost=-1;

    int cost;

    getSetGo();
    for(map<int,string>::iterator it=optimal.setOfStringsInMap.begin ();it!=optimal.setOfStringsInMap.end ();it++)
    {
        cout<<it->second<<endl;
    }
    
    return 0;
}
