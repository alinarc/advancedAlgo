// Code to read graph instances from a file.  Uses the Boost Graph Library (BGL).

#include <iostream>
#include <limits.h>
#include "d_except.h"
#include <fstream>

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
void setNodeWeights(Graph &g, vector <int> colors, int w);
void printSolution(Graph &g, int numConflicts);
int calculateNumConflicts(Graph &g);
void convertToBaseK(int num, vector<int> &bin, int k);
//void computeVertexDegrees(Graph &g);
//void sortVerticesByDegree(Graph &g, vector <Vertex> &nodes);

struct VertexProperties
{
    pair<int,int> cell; // maze cell (x,y) value
    Graph::vertex_descriptor pred;
    bool visited;
    bool marked;
    int weight;
    int color;
    //int degree;
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
    
    fileName = "color/colortest.input";
    
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
        initializeGraph(g,fin);

        
        cout << "Num nodes: " << num_vertices(g) << endl;
        cout << "Num edges: " << num_edges(g) << endl;
        cout << endl;

        int num = 15;
        int k = 3;
        vector <int> bin;
        bin.resize(0);
        convertToBaseK(num, bin, 3);
        cout << "base-" << k << " representation of " << num << " is: " << endl;
        for (int i = 0; i < bin.size(); i++)
        {
            cout << bin.at(i) << " ";
        }
        cout << endl;
        //binaryCounter(2);

        // cout << g;
        
        //numConflicts = exhaustiveColoring(g, numColors, 600);
        //printSolution(g, numConflicts);
        
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
{

}

void initializeGraph(Graph &g, ifstream &fin)
// Initialize g using data from fin.
{
    int n, e;
    int j,k;
    
    fin >> n >> e;
    Graph::vertex_descriptor v;
    
    // Add nodes.
    for (int i = 0; i < n; i++)
        v = add_vertex(g);
    
    for (int i = 0; i < e; i++)
    {
        fin >> j >> k;
        add_edge(j,k,g);  // Assumes vertex list is type vecS
        //add_edge(k, j, g); // Added this to correctly comput the degree of each vertex. Not sure why its necessary, since the graph is biDirectional. May be unneccessary or there may be a better way
    }
    //computeVertexDegrees(g);
}

/* void computeVertexDegrees(Graph &g)
{
    pair <vertex_iterator, vertex_iterator> vItrRange = vertices(g);
    for (vertex_iterator vItr = vItrRange.first; vItr != vItrRange.second; ++vItr)
    {
        int count = 0;
        pair <adj_iterator, adj_iterator> adjItrRange = adjacent_vertices(*vItr, g);
        for (adj_iterator adjItr = adjItrRange.first; adjItr != adjItrRange.second; ++adjItr)
        {
            count = count + 1;
        } 
        g[*vItr].degree = count;
    }
}
*/

/* void sortVerticesByDegree(Graph &g, vector <Vertex> &nodes)
{
    //nodes.resize(num_vertices(g));
    int currMax;
    
    pair <vertex_iterator, vertex_iterator> vItrRange = vertices(g);
    for (vertex_iterator vItr = vItrRange.first; vItr != vItrRange.second; ++vItr)
    {
        if (g[*vItr].degree > currMax)
        {
            currMax = g[*vItr].degree;
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
} */

void setNodeWeights(Graph &g, vector <int> colors, int w)
// Set all node weights to w.
{
    pair<Graph::vertex_iterator, Graph::vertex_iterator> vItrRange = vertices(g);
    
    for (Graph::vertex_iterator vItr= vItrRange.first; vItr != vItrRange.second; ++vItr)
    {
        g[*vItr].weight = w;
    }
}

void printSolution(Graph &g, int numConflicts)//, string filePath_output)
{
    //ofstream myfile;

    //myfile.open(filePath_output.c_str());

    cout << "Total Conflicts: " << numConflicts << endl;

    for (int counter = 0; counter < num_vertices(g); counter++)
    {
        cout << counter << ": " << g[counter].color << endl;
    }
    //myfile.close();
}

void convertToBaseK(int num, vector <int> &bin, int k) // Converts num to base-k representation, stores each digit in bin
{
    if (num < k)
    {
        bin.push_back(num);
    }
    else
    {
        convertToBaseK(num/k, bin, k);
        bin.push_back(num%k);

    }
}



int calculateNumConflicts(Graph &g)
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

