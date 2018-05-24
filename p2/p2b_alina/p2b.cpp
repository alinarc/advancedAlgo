// Project 2b: Graph coloring using greedy algorithm
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
int greedyColoring(Graph &g, int numColors, int t, vector <Vertex> sortedNodes);
int sumOfVectorElements(vector <int> v);
void setNodeWeights(Graph &g, vector <int> colors);
int calculateNumConflicts(Graph &g);
void baseKIncrement(int numColors, vector<int> &v);
void printSolution(Graph &g, int numConflicts, int numColors);
vector <Vertex> sortVerticesByDegree(Graph &g);
void swap(int v1, int v2, vector <Vertex> &v);
void printVerticesAndDegrees(vector<Vertex> &v, Graph &g);
void color(Vertex &v, Graph &g, int color);
void uncolor(Vertex &v, Graph &g);
bool areAdjacent(Vertex &v1, Vertex &v2, Graph &g);


struct VertexProperties
{
	pair<int, int> cell; // maze cell (x,y) value
	Graph::vertex_descriptor pred;
	bool colored;
	int degree;
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

	fileName = "color/color12-3.input";

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

		vector <Vertex> verts = sortVerticesByDegree(g);

		printVerticesAndDegrees(verts, g);


		cout << "Num nodes: " << num_vertices(g) << endl;
		cout << "Num edges: " << num_edges(g) << endl;
		cout << endl;

		greedyColoring(g, numColors, 600, verts);
		//numConflicts = exhaustiveColoring(g, numColors, 600);
		//printSolution(g, numConflicts, numColors);

	}
	catch (indexRangeError &ex)
	{
		cout << ex.what() << endl; exit(1);
	}
	catch (rangeError &ex)
	{
		cout << ex.what() << endl; exit(1);
	}
	//system("pause");
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
		cout << " adding vertex " << i << endl;
		v = add_vertex(g);
	}
		

	for (int i = 0; i < e; i++)
	{
		fin >> j >> k;
		add_edge(j, k, g);  // Assumes vertex list is type vecS
		g[j].degree = g[j].degree + 1;
		g[k].degree = g[k].degree + 1;
	}

}

int greedyColoring(Graph &g, int numColors, int t, vector <Vertex> sortedNodes)
{
	int n = 0;
	for (int i = 0; i < sortedNodes.size(); i++)
	{
		if (n < numColors)
		{
			if (!g[sortedNodes.at(i)].colored)
			{
				color(sortedNodes.at(i), g, n);
				for (int j = i + 1; j < sortedNodes.size(); j++)
				{
					if (!areAdjacent(sortedNodes.at(i), sortedNodes.at(j), g))
						color(sortedNodes.at(j), g, n);
				}
			}
		}
		n+=1;
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

vector<Vertex> sortVerticesByDegree(Graph &g)
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

/*     for (vertex_iterator vItr = vItrRange.first; vItr != vItrRange.second - 1; ++vItr)
    {
		currMax = *vItr;
		for (vertex_iterator vItr2 = vItr+1; vItr2 < vItrRange.second; ++vItr2)
		{
			if (g[*vItr2].degree > g[currMax].degree)
			{
				currMax = *vItr2;
			}
			swap(currMax, *vItr, nodes);
		}
        
    }
    for (int deg = currMax; deg >= 0; deg--)
    {
        for (vertex_iterator vItr = vItrRange.first; vItr != vItrRange.second; ++vItr)
        {
            if (g[*vItr].degree == deg)
            {
                nodes.push_back(*vItr);
            }
        }
	}
	*/
	return nodes;
}

void swap(int v1, int v2, vector <Vertex> &v)
{
	Vertex temp = v.at(v1);
	v.at(v1) = v.at(v2);
	v.at(v2) = temp;
}

void printVerticesAndDegrees(vector<Vertex> &v, Graph &g)
{
	for (int i = 0; i < num_vertices(g); i++)
	{
		cout << v.at(i) << ": " << g[v.at(i)].degree << endl;
	}
}

void color(Vertex &v, Graph &g, int color)
{
	cout << "Coloring vertex " << v << " with color " << color << endl;
 	g[v].color = color;
	g[v].colored = true;
}

void uncolor(Vertex &v, Graph &g)
{
	g[v].colored = false;
}

bool areAdjacent(Vertex &v1, Vertex &v2, Graph &g)
{
	pair <Edge, bool> checkEdge = edge(v1, v2, g);
	if (checkEdge.second)
		return true;
	else return false;
}

bool isAdjacentToColor(Vertex &v, Graph &g, int color)
{
	pair <vertex_iterator, vertex_iterator> vItrRange = vertices(g);
	for (vertex_iterator vItr = vItrRange.first; vItr != vItrRange.second; ++vItr)
	{
		if (g[vItr].color == color)
		{
			
		}
	}

}
