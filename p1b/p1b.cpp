// Code to read graph instances from a file.  Uses the Boost Graph Library (BGL).

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

int exhaustiveColoring(Graph &g, int numColors, int t);
void initializeGraph(Graph &g, ifstream &fin);
void setNodeWeights(Graph &g, vector <int> colors);
void printSolution(Graph &g, int numConflicts, int numColors);
int calculateNumConflicts(Graph &g);
void convertToBaseK(int num, vector<int> &bin, int k);
int sumOfVectorElements(vector <int> v);

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
	/*vector<int> testV (5,0);
	for (int j = 0; j < 20; j++)
	{
		binaryIncrement(3, testV);
		for (int i = 0; i < 5; i++)
		{
			cout << testV.at(i);
		}
		cout << endl;
	}*/

	char x;
	ifstream fin;
	string fileName;

	// Read the name of the graph from the keyboard or
	// hard code it here for testing.

	fileName = "color/color48-5.input";

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

int exhaustiveColoring(Graph &g, int numColors, int t)
// Function that iterates through all possible coloring assignments by using the fact that an integer can represent a coloring of size k by converting the integer to base-k representation. By iterating from 0 to 'kkkk' (for a graph with 4 vertices, for example), we iterate through all possible vertex colorings. We then calculate the number for conflicts for each one and store the minimum.
{
    clock_t startTime = clock();

    int size
    ces(g);
    int minConflicts = LargeValue;
    int maxSum = (numColors - 1) * size;
    vector <int> colors;
    for (int i = 0; i < size; i++)
    {
        colors.push_back(0);
    }

    while (sumOfVectorElements(colors) < maxSum)
    {
        int diff = clock() - startTime;
        int runTime = diff / CLOCKS_PER_SEC;
        if (runTime <= t) // Checks that runTime is less than the time limit
        {
            bin.clear();
            convertToBaseK(count, bin, numColors);
            setNodeWeights(g, bin);
            int numConflicts = calculateNumConflicts(g);

            if (numConflicts < minConflicts) // Store minimum num conflicts and the integer that corresponds to it
            {
                minConflicts = numConflicts;
                numberForMin = count;
            }
        }
        else // Breaks loop if runTime has exceeded time limit
        {   
            cout << "time is " << runTime << endl;
            break;
        }
    }

    bin.clear();
    convertToBaseK(numberForMin, bin, numColors); // Assigns the graph the coloring that gives the minimum number of conflicts
    setNodeWeights(g, bin);
    minConflicts = calculateNumConflicts(g);
    return minConflicts;

}
void binaryIncrement(int numColors, vector<int> &basedVector)
{
	int lengthVector;
	lengthVector = basedVector.size();
	basedVector.at(0) = basedVector.at(0) + 1;
	for (int i = 0; i < lengthVector; i++)
	{
		if (basedVector.at(i) == numColors)
		{
			basedVector.at(i) = 0;
			basedVector.at(i + 1) = basedVector.at(i + 1) + 1;
		}

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


void setNodeWeights(Graph &g, vector <int> colors)
// Set node weights to the values specified in the vector colors
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

void convertToBaseK(int num, vector <int> &bin, int k)
// Convert num to base-k representation, stores each digit in bin
{
	if (num < k)
	{
		bin.push_back(num);
	}
	else
	{
		convertToBaseK(num / k, bin, k);
		bin.push_back(num%k);
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

int sumOfVectorElements(vector <int> v)
{
    int sum = 0;
    for (int i = 0; i < v.size(); i++)
    {
        sum += v.at(i);
    }
}