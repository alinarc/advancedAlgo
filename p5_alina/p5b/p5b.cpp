// Project 5b: Graph coloring using steepest descent & simulated annealing
//  Alina Rossi-Conaway & Alexander Duffy

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


int greedyColoring(Graph &g, int numColors);
int steepestDescent(Graph &g, int &numColors, int t);
int anneal(Graph &g, int &numColors, const int &timeLimit);

int generateRandomSolution(Graph &g, int &numColors);
int generateMinNeighbor(Graph &g, int &numColors);
Graph generateRandomNeighbor( Graph &g, int &numColors);

int randNum(int min, int max);
double acceptanceProb(double &oldVal, double &newVal, double &T);

void initializeGraph(Graph &g, ifstream &fin);
int calculateNumConflicts(Graph &g);
void printSolution(Graph &g, int numConflicts, int numColors);
void writeToCompare(Graph &g, int numColors, int greedy, int steep, int annealVal);
vector <Vertex> sortVerticesByDegree(Graph &g);
void swap(int v1, int v2, vector <Vertex> &v);
void color(Vertex &v, Graph &g, int color);
bool isAdjacentToColor(Vertex &v, Graph &g, int color);

struct VertexProperties
{
	bool colored;
	int degree;
	int color;
};

// Create a struct to hold properties for each edge
struct EdgeProperties
{};

int main()
{
  srand(unsigned(time(NULL)));
	char x;
	ifstream fin;
	string fileName;

	// Read the name of the graph from the keyboard or
	// hard code it here for testing.

	fileName = "color/color192-8.input";

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

		int greedy = greedyColoring(g, numColors);
		cout << "Initial solution found from greedy algorithm: " << endl; 
		printSolution(g, greedy, numColors);

    int steep = steepestDescent(g, numColors, 300);
    cout << "Steepest descent solution: " << endl;
    printSolution(g, steep, numColors);

    int annealSol = anneal(g, numColors, 300);
    cout << "Simulated annealing solution: " << endl;
    printSolution(g, annealSol, numColors);

    writeToCompare(g, numColors, greedy, steep, annealSol);
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

int steepestDescent(Graph &g, int &numColors, int t)
{
  clock_t startTime = clock();
  int runTime;
  
  Graph curr = g;
    int currCon = calculateNumConflicts(g);
  Graph min = g;
    int minCon = calculateNumConflicts(min);
    int nextCon;

  while (currCon != nextCon && runTime <= t)
  {
    cout << "generating neighbor " << endl;
    runTime = (clock() - startTime)/CLOCKS_PER_SEC;

    currCon = calculateNumConflicts(g); // Stores in currCon the value of graph g
    nextCon = generateMinNeighbor(g, numColors); // Updates g to hold its minimum-valued neighbor, stores in nextCon its value
    
    if (nextCon < minCon)
    // Only accept neighbors if they are better than current best
    {
      min = g;
      minCon = nextCon;
    }
    else currCon = nextCon; // Breaks loop otherwise
  }
  g = min;
  return calculateNumConflicts(g);
  cout << "runTime is " << runTime << endl;
}

int anneal(Graph &g, int &numColors, const int &timeLimit)
{
  clock_t startTime = clock();
  int runTime; 
  //int minCon = greedyColoring(g, numColors);
  int minCon = generateRandomSolution(g, numColors);
  Graph minGraph = g;
  cout << "Randomly generated solution: " << endl;
  printSolution(g, minCon, numColors);

  double oldCon = minCon;
  double t = 1;
  double tMin = 0.0001;
  double alpha = 0.9;
  
  while (t >= tMin && runTime <= timeLimit)
  {
    int i = 1;
    runTime = (clock() - startTime)/CLOCKS_PER_SEC;

    while (i <= 1000 && runTime <= timeLimit)
    {
      runTime = (clock() - startTime)/CLOCKS_PER_SEC;

      Graph newSol = generateRandomNeighbor(g, numColors);

      double newCon = calculateNumConflicts(newSol);
      double ap = acceptanceProb(oldCon, newCon, t);
      double randProb = (rand() % (100000)) / 100000.0;

      if (ap >= randProb)
      {
        g = newSol;
        if (newCon <= minCon)
        {
          cout << "found solution with " << minCon << " conflicts" << endl; // Testing--print when we reach a current best solution
          minGraph = newSol;
          minCon = newCon;
        }

        if (ap != 1) // Testing--print instances where we accept solutions that are worst than g
        {
          cout << "ap is " << ap << endl;
          cout << "randProb is " << randProb << endl;
          cout << "t is " << t << endl;
          cout << "min is " << minCon << endl;
          cout << "accepting solution with  " << newCon << " conflicts" << endl << endl; 
        }

        oldCon = newCon;
      }
      i += 1;
    }
    t = t*alpha;
  }

  cout << "runTime is " << runTime << endl;
  g = minGraph;
  return calculateNumConflicts(g);

}

int generateRandomSolution(Graph &g, int &numColors)
// Generates a random solution by coloring each node a random color. Stores this solution in Graph g.
{
  pair <vertex_iterator, vertex_iterator> vItrRange = vertices(g);
  for (vertex_iterator vItr = vItrRange.first; vItr != vItrRange.second; ++vItr)
  {
    int col = randNum(0, numColors);
    g[*vItr].color = col;
  }
  return calculateNumConflicts(g);
}

Graph generateRandomNeighbor(Graph &g, int &numColors)
// Generates a random neighbor of Graph g. Returns this neighbor.
{
  Graph neighb = g;
  int randNode = randNum(0, num_vertices(g));
  int randCol = randNum(0, numColors);
  neighb[randNode].color = randCol;
  return neighb;
}

int generateMinNeighbor(Graph &g, int &numColors)
// Generates all neighbors that come from changing one node's color. Stores the neighbor with the minimum 
// number of conflicts in graph g and returns its conflict count.
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

int randNum(int min, int max)
// Return a random number on the interval [min, max)
{
  int x = rand() % max + min;
  return x;
}

double acceptanceProb(double &oldVal, double &newVal, double &T)
// Calculates probability of acceptance
{
  //cout << "newVal is "<< newVal <<", oldVal is " << oldVal << endl;
  double e = (oldVal-newVal)/T;
  //cout << "exp is " << e << endl;
  double prob = exp(e);
  //cout << "prob is "<< prob << endl;
  if (newVal <= oldVal) return 1;
  //if (prob == 1) return 0; 
  else return prob; 
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
	myfile.open("output/simulatedAnnealing/color" + to_string(num_vertices(g)) + "-" + to_string(numColors) + ".output");

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

void writeToCompare(Graph &g, int numColors, int greedy, int steep, int annealVal)
{
  ofstream outFile;
  outFile.open("output/compare.txt", ios::out | ios::app);
  outFile << "  " << num_vertices(g) << "-" << numColors << "   ||	" << greedy << "            ||	      " << steep << "	         ||        " << annealVal << endl;
  outFile << "-----------------------------------------------------------------------" << endl;
}
