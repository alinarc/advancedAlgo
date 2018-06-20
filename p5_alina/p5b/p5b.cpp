// Project 5b: Graph coloring using steepest descent
// Alexander Duffy & Alina Rossi-Conaway

#include <iostream>
#include <string>
#include <limits.h>
#include <fstream>
#include <time.h>
#include "d_except.h"
#include <vector>

#include <boost/graph/adjacency_list.hpp>

#define LargeValue 99999999

using namespace std;
using namespace boost;

int const NONE = -1;  // Used to represent a node that does not exist

struct VertexProperties;
struct EdgeProperties;

typedef adjacency_list<vecS, vecS, undirectedS, VertexProperties, EdgeProperties> Graph;
typedef Graph::vertex_descriptor Vertex;
typedef Graph::edge_descriptor Edge;
typedef Graph::edge_iterator edge_iterator;
typedef Graph::vertex_iterator vertex_iterator;
typedef Graph::adjacency_iterator adj_iterator;

void initializeGraph(Graph &g, ifstream &fin);
int greedyColoring(Graph &g, int numColors);
int calculateNumConflicts(Graph &g);
void printSolution(Graph &g, int numConflicts, int numColors);
vector <Vertex> sortVerticesByDegree(Graph &g);
void swap(int v1, int v2, vector <Vertex> &v);
void color(Vertex &v, Graph &g, int color);
bool isAdjacentToColor(Vertex &v, Graph &g, int color);
int generateNeighbors(Graph &g, int &numColors);


struct VertexProperties
{
	bool colored;
	int degree;
	int color;
};

// Create a struct to hold properties for each edge
struct EdgeProperties
{
};

int main()
{
	char x;
	ifstream fin;
	string fileName;

	// Read the name of the graph from the keyboard or
	// hard code it here for testing.

	fileName = "color/color192-6.input";

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

		initializeGraph(g, fin);

		cout << "Num nodes: " << num_vertices(g) << endl;
		cout << "Num edges: " << num_edges(g) << endl;
		cout << endl;

		numConflicts = greedyColoring(g, numColors);
		cout << "Initial solution found from greedy algorithm: " << endl; 
		printSolution(g, numConflicts, numColors);
    
    clock_t startTime = clock();
    int runTime;
    Graph curr;
    while (calculateNumConflicts(g) != calculateNumConflicts(curr))
    {
      runTime = (clock() - startTime)/CLOCKS_PER_SEC;
      if (runTime <= 300)
      {
        curr = g;
        numConflicts = generateNeighbors(g, numColors);
      }
      else break;
    }		
		cout << "New solution from steepest descent: " << endl;
    printSolution(g, numConflicts, numColors);
    
    cout << "runTime: " << runTime << " seconds" << endl;
		
		
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

int generateNeighbors(Graph &g, int &numColors)
{
	Graph currMin = g;
	pair <vertex_iterator, vertex_iterator> vItrRange = vertices(g);
	for (vertex_iterator vItr = vItrRange.first; vItr != vItrRange.second; ++vItr)
	{
		for (int i = 0; i < numColors; i++)
		{
			Graph test;
			test = g;
			if (g[*vItr].color != i)
				test[*vItr].color = i;
			if (calculateNumConflicts(test) < calculateNumConflicts(currMin))
				currMin = test;
		}
	}
	g = currMin;
	return calculateNumConflicts(g);
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
	{
		v = add_vertex(g);
		g[v].degree = 0;
		g[v].colored = false;
		g[v].color = NONE;
	}
		
	// Add edges and compute vertex degree for each node.
	for (int i = 0; i < e; i++)
	{
		fin >> j >> k;
		add_edge(j, k, g);  // Assumes vertex list is type vecS
		g[j].degree = g[j].degree + 1;
		g[k].degree = g[k].degree + 1;
	}
}

int greedyColoring(Graph &g, int numColors)
// Greedy algorithm that colors nodes one at a time based on the degree of the vertex.
{
	vector <Vertex> sortedNodes = sortVerticesByDegree(g);
	int n = 0;
	while (n < numColors)
	{
		for (int i = 0; i < sortedNodes.size(); i++)
		{
			if (n < numColors - 1)
			{
				if (!g[sortedNodes.at(i)].colored && !isAdjacentToColor(sortedNodes.at(i), g, n))
				{
					color(sortedNodes.at(i), g, n);	
				}
			}
			else if (n == numColors - 1)
			{
				if(!g[sortedNodes.at(i)].colored)
					color(sortedNodes.at(i), g, n);	
			}
		}
		n+=1;
	}
	return calculateNumConflicts(g);
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
// Print coloring solution and write it to an output file
{
	ofstream myfile;
	int size = num_vertices(g);
	myfile.open("output/color" + to_string(num_vertices(g)) + "-" + to_string(numColors) + ".output");

	cout << "Total Conflicts: " << numConflicts << endl;
	myfile << "Total Conflicts: " << numConflicts << endl;

	for (int counter = 0; counter < num_vertices(g); counter++)
	{
		cout << counter << ": " << g[counter].color << endl;
		myfile << counter << ": " << g[counter].color << endl;
	}
	myfile.close();
}

vector<Vertex> sortVerticesByDegree(Graph &g)
// Returns a vector containing all nodes in g sorted by degree
{
	vector <Vertex> nodes;
    nodes.resize(num_vertices(g));
    int i, j, currMax;
    
	pair <vertex_iterator, vertex_iterator> vItrRange = vertices(g);
	
	for (vertex_iterator vItr = vItrRange.first; vItr != vItrRange.second; ++vItr)
	{
		nodes.at(*vItr) = *vItr;
	}
	
	for (i = 0; i < nodes.size() - 1; i++)
	{
		currMax = i;
		for (j = i+1; j < nodes.size(); j++)
		{
			if (g[nodes.at(j)].degree > g[nodes.at(currMax)].degree)
				currMax = j;	
		}
		swap(currMax, i, nodes);
	}
	return nodes;
}

void swap(int v1, int v2, vector <Vertex> &v)
// Helper function for sorting
{
	Vertex temp = v.at(v1);
	v.at(v1) = v.at(v2);
	v.at(v2) = temp;
}

void color(Vertex &v, Graph &g, int color)
// Colors vertex v with color 'color'
{
 	g[v].color = color;
	g[v].colored = true;
}

bool isAdjacentToColor(Vertex &v, Graph &g, int color)
// Returns true if vertex v is adjacent to any node with color 'color'. Otherwise returns false
{
	pair <vertex_iterator, vertex_iterator> vItrRange = vertices(g);
	for (vertex_iterator vItr = vItrRange.first; vItr != vItrRange.second; ++vItr)
	{
		if (g[*vItr].color == color)
		{
			pair <Edge, bool> checkEdge = edge(*vItr, v, g);
			if (checkEdge.second)
				return true;
		}
	}
	return false;
}
