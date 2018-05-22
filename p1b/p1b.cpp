// Project 1b: Graph coloring using exhaustive algorithm
// Alina Rossi-Conaway & Alexander Duffy

#include <iostream>
#include <string>
#include <limits.h>
#include <fstream>
#include <time.h>
#include "d_except.h"

#include <boost/graph/adjacency_list.hpp>

#define LargeValue 99999999

using namespace std;
using namespace boost;

int const NONE = -1;  // Used to represent a node that does not exist

struct VertexProperties;
struct EdgeProperties;

typedef adjacency_list<vecS, vecS, bidirectionalS, VertexProperties, EdgeProperties> Graph;
typedef Graph::vertex_descriptor Vertex;
typedef Graph::edge_descriptor Edge;
typedef Graph::edge_iterator edge_iterator;
typedef Graph::vertex_iterator vertex_iterator;
typedef Graph::adjacency_iterator adj_iterator;

void initializeGraph(Graph &g, ifstream &fin);
int exhaustiveColoring(Graph &g, int numColors, int t);
int sumOfVectorElements(vector <int> v);
void setNodeWeights(Graph &g, vector <int> colors);
int calculateNumConflicts(Graph &g);
void baseKIncrement(int numColors, vector<int> &v);
void printSolution(Graph &g, int numConflicts, int numColors);

struct VertexProperties
{
	pair<int, int> cell; // maze cell (x,y) value
	Graph::vertex_descriptor pred;
	bool visited;
	bool marked;
	int weight;
	int color;
};

// Create a struct to hold properties for each edge
struct EdgeProperties
{
	int weight;
	bool visited;
	bool marked;
};

int main()
{
	char x;
	ifstream fin;
	string fileName;

	// Read the name of the graph from the keyboard or
	// hard code it here for testing.

	fileName = "color/color24-5.input";

	//   cout << "Enter filename" << endl;
	//   cin >> fileName;

	fin.open(fileName.c_str());
	if (!fin)
	{
		cerr << "Cannot open " << fileName << endl;
		exit(1);
	}

	try
	{
		cout << "Reading graph" << endl;
		Graph g;
		int numColors;
		int numConflicts = -1;
		fin >> numColors;
		cout << "Num colors: " << numColors << endl;
		initializeGraph(g, fin);

		cout << "Num nodes: " << num_vertices(g) << endl;
		cout << "Num edges: " << num_edges(g) << endl;
		cout << endl;

		numConflicts = exhaustiveColoring(g, numColors, 600);
		printSolution(g, numConflicts, numColors);

	}
	catch (indexRangeError &ex)
	{
		cout << ex.what() << endl; exit(1);
	}
	catch (rangeError &ex)
	{
		cout << ex.what() << endl; exit(1);
	}
}

void initializeGraph(Graph &g, ifstream &fin)
// Initialize g using data from fin.
{
	int n, e;
	int j, k;

	fin >> n >> e;
	Graph::vertex_descriptor v;

	// Add nodes.
	for (int i = 0; i < n; i++)
		v = add_vertex(g);

	for (int i = 0; i < e; i++)
	{
		fin >> j >> k;
		add_edge(j, k, g);  // Assumes vertex list is type vecS
	}
}

int exhaustiveColoring(Graph &g, int numColors, int t)
/* 
    Iterates through all possible colorings by using the fact that an 
    integer can represent a coloring of size k by converting the integer to 
    base-k representation. Starts from 0 and repeatedly calls the function 
    baseKIncrement to increment the base-k number stored in vector 'colors', 
    thus coming up with a new coloring. Calls calculateNumConflicts for each 
    assignment, and stores the minimum number of conflicts.
*/
{
    clock_t startTime = clock();

    int size = num_vertices(g);
    int minConflicts = LargeValue;
    int maxSum = (numColors - 1) * size; // Maximum sum of elements in vector 'colors'


    vector <int> colors(size, 0); // Will store each coloring assignment
    vector <int> vectorForMin(size); // Will store coloring assignment with minimum conflicts

    while (sumOfVectorElements(colors) < maxSum) // While we still have legal colorings to evaluate
    {
        int diff = clock() - startTime;
        int runTime = diff / CLOCKS_PER_SEC;
        if (runTime <= t) // Checks that runTime is less than the time limit
        {
            setNodeWeights(g, colors);
            int numConflicts = calculateNumConflicts(g);

            if (numConflicts < minConflicts) // Store minimum num conflicts and the vector that corresponds to it
            {
                minConflicts = numConflicts;
                for (int i = 0; i < size; i++)
                {
                    vectorForMin.at(i) = colors.at(i);
                }
            }
            baseKIncrement(numColors, colors);
        }
        else break; // Breaks loop if runTime has exceeded time limit
    }

    setNodeWeights(g, vectorForMin); // Assign coloring with minimum weight
    minConflicts = calculateNumConflicts(g);
    return minConflicts;
}

int sumOfVectorElements(vector <int> v)
// Sum the contents of vector 'v'
{
    int sum = 0;
    for (int i = 0; i < v.size(); i++)
    {
        sum += v.at(i);
    }
    return sum;
}

void baseKIncrement(int numColors, vector<int> &v)
/*
    Takes in a vector whose components represent a coloring assigment (i.e. an 
    integer in base-k representation) where k = numColors, and increments it to 
    generate a new coloring assignment
*/
{
	int size= v.size();
	v.at(0) = v.at(0) + 1;
	for (int i = 0; i < size; i++)
	{
		if (v.at(i) == numColors)
		{
			v.at(i) = 0;
			v.at(i + 1) = v.at(i + 1) + 1;
		}
    }
}

void setNodeWeights(Graph &g, vector <int> colors)
// Set node weights to the values specified in the vector 'colors'
{
	int n = num_vertices(g);

	for (int i = 0; i < n; i++)
	{
		if (i < colors.size())
		{
			g[i].color = colors.at(i);
		}
		else
		{
			g[i].color = 0;
		}
	}
}

int calculateNumConflicts(Graph &g)
// Calculate the number of conflicts in Graph g
{
	int numConflicts = 0;

	pair <edge_iterator, edge_iterator> eItrRange = edges(g);
	for (edge_iterator eItr = eItrRange.first; eItr != eItrRange.second; ++eItr)
	{
		if (g[target(*eItr, g)].color == g[source(*eItr, g)].color)
		{
			numConflicts = numConflicts + 1;
		}
	}
	return numConflicts;
}

void printSolution(Graph &g, int numConflicts, int numColors)
// Print coloring solution as well as write it to an output file
{
	ofstream myfile;
	int size = num_vertices(g);
	myfile.open("outputs/color" + to_string(num_vertices(g)) + "-" + to_string(numColors) + ".output");


	cout << "Total Conflicts: " << numConflicts << endl;
	myfile << "Total Conflicts: " << numConflicts << endl;

	for (int counter = 0; counter < num_vertices(g); counter++)
	{
		cout << counter << ": " << g[counter].color << endl;
		myfile << counter << ": " << g[counter].color << endl;
	}
	myfile.close();
}
