// Project 5a: Solving knapsack using steepest descent
// Alina Rossi-Conaway & Alexander Duffy

#include <iostream>
#include <limits.h>
#include <list>
#include <fstream>
#include <queue>
#include <stack>
#include <vector>
#include <time.h>
#include <string>
#include <cmath>

using namespace std;

#include "d_except.h"
#include "d_matrix.h"
#include "knapsack.h"
#include "d_random.h"

struct greaterThanByValue
{
  bool operator() (const knapsack &k1, const knapsack &k2)
  {
    return k1.getValue() < k2.getValue();
  }
};

void greedyKnapsack(knapsack &k);
void steepestDescent(knapsack &k, const int t);
void generateMaxNeighbor(knapsack &k);
void generateMaxNeighbor1(knapsack &k, vector <knapsack> neighbors);
void generateRandomSolution(knapsack &k);
knapsack generateRandomNeighbor(knapsack &k);
double acceptanceProb(double &oldVal, double &newVal, double &T);
int randNum(int min, int max);
void anneal(knapsack &k, const int time);
vector <knapsack> generateNeighbors(knapsack &k);
void generateOneNeighbor(knapsack &k, int i, int j, vector <knapsack> &neighbors);
void writeToCompare(knapsack &greedy, knapsack &steep, knapsack &anneal);

int main()
{
  srand(unsigned(time(NULL)));
  char x;
  ifstream fin;
  stack <int> moves;
  string fileName;
  vector <knapsack> pos;
   
   // Read the name of the graph from the keyboard or
   // hard code it here for testing.
   
   fileName = "knapsack/knapsack1024.input";

   /* cout << "Enter filename" << endl;
   cin >> fileName; */
   
   fin.open(fileName.c_str());
   if (!fin)
   {
      cerr << "Cannot open " << fileName << endl;
      exit(1);
   }
   try
   {
      cout << "Reading knapsack instance" << endl;
      knapsack k(fin);

      cout << k << endl;

      /* greedyKnapsack(k);

      cout << endl << "Initial solution:" << endl;
      k.printSolution();

      cout << endl << "Steepest descent solution:" << endl;
      k.printSolution(); */
      
    greedyKnapsack(k);
    knapsack greedy = k;

    cout << "Greedy solution: " << endl;
    k.printSolution();

    steepestDescent(k, 300);
    cout << "Steepest descent solution: " << endl;
    knapsack steep = k;
    k.printSolution();

    anneal(k, 300);
    knapsack anneal = k;
    k.printSolution();

    writeToCompare(greedy, steep, anneal);
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

void greedyKnapsack(knapsack &k)
// Greedy algorithm to find optimal knapsack solution. Considers items in 
// decreasing order of ratio (of value to cost), adds them if they don't 
// create a conflict (exceed costLimit).
{
    int i = 0;
    while (k.getCost() + k.getCost(k.getVector(i).at(0)) <= k.getCostLimit() && i < k.getNumObjects())
    {
        k.select(k.getVector(i).at(0));
        i+=1;
    }
}

void steepestDescent(knapsack &k, const int t)
// Explores all neighbors of knapsack k using generateNeighbors(k) function. 
// Calls generateMaxNeighbor1 function to take the max neighbor from the pq 
// on each iteration
{
  clock_t startTime = clock();
  int runTime;
  knapsack max;
  knapsack curr;
  while (curr != k && runTime <= t) // Uses overloaded operator that compares selected vectors
  {
    runTime = (clock()-startTime)/CLOCKS_PER_SEC;
    cout << "runTime is " << runTime << endl;
    curr = k;
    //vector <knapsack> n = generateNeighbors(k);

    generateMaxNeighbor(k);
    if (k.getValue() > max.getValue())
      max = k;
  }
  k = max;

}

void anneal(knapsack &k, const int time)
// Simulated annealing to find optimal solution for knapsack k in time limit.
// Starts with a random solution, explores neighbors and randomly accepts them
// based on their closeness to the current value
{
  //greedyKnapsack(k);
  generateRandomSolution(k);
  knapsack maxKnap = k;
  int maxVal = maxknap.getValue();
  clock_t startTime = clock();
  int runTime;
  cout << endl << "Randomly generated solution: " << endl;
  k.printSolution();
  
/*   cout << "greedy solution: " << endl;
  maxKnap.printSolution(); */
  cout << "max val: " << maxKnap.getValue() << endl;;
  double oldVal = k.getValue();
  double t = 1000.0;
  double tMin = 0.00001;
  double alpha = 0.9;
  while (t >= tMin && runTime <= time)
  {
    int i = 1;
    runTime = (clock() - startTime)/CLOCKS_PER_SEC;
    while (i <= 1000 && runTime <= time)
    {
      runTime = (clock() - startTime)/CLOCKS_PER_SEC;
      
      knapsack newSol = generateRandomNeighbor(k);
      
      double newVal = newSol.getValue();
      double ap = acceptanceProb(oldVal, newVal, t);
      double randProb = (rand() % (10000)) / 10000.0;

      if (ap >= randProb)
      {
        k = newSol;
        if (newVal > maxVal)
        {
          maxKnap = newSol;
          maxVal = maxKnap.getValue();
        }

        if (ap != 1) // testing--print instances where we accept solutions that are worse than current k
        {
          cout << "ap is " << ap << endl;
          cout << "randProb is " << randProb << endl;
          cout << "t is " << t << endl;
          cout << "accepting solution with value " << newVal << endl << endl; 
        }

        oldVal = newVal;
      }
      i += 1;
      }

    t = t * alpha;
  }
  cout << "runTime is " << runTime << endl;
  k = maxKnap;
}

void generateMaxNeighbor(knapsack &k)
{
  knapsack kmax = k;
  for (int i = 0; i < k.getNumObjects(); i++)
  {
    knapsack test(k);
    if (k.isSelected(i))
      test.unSelect(i);
    else test.select(i);

    if (test.getCost() <= test.getCostLimit() && test.getValue() > kmax.getValue())
      kmax = test;
  }
  k = kmax;
}

void generateMaxNeighbor1(knapsack &k, vector <knapsack> neighbors)
// Takes in neighbors in vector and stores them in a priority queue, allowing the 
// max one to be stored in k 
{
  priority_queue <knapsack, vector <knapsack>, greaterThanByValue> pq;
  for (int i = 0; i < neighbors.size(); i++)
  {
    pq.push(neighbors.at(i));
    //cout << "adding neighbor with value " << neighbors.at(i).getValue() << endl;
  }
  k = pq.top();
  //cout << endl << "max neighbor has value " << k.getValue() << endl;
}

vector <knapsack> generateNeighbors(knapsack& k)
// Generates k's neighbors by creating combinations of two items and calling generateOneNeighbor for all of those combinationseither:
//  1. If both items are unselected: selects one or both if they fit, or
//  2. If only one item is selected: flips selected and unselected if the new item fits
{
  vector <knapsack> neighbors;

  for (int i = 0; i < k.getNumObjects(); i++)
  {
    for (int j = i + 1; j < k.getNumObjects(); j++)
    //Select all pairs of two objects
    {
      generateOneNeighbor(k, i, j, neighbors);
    }
  }
  return neighbors;
}


void generateRandomSolution(knapsack &k)
// Generates random solution to use as initial solution in simulated annealing
{
  randomNumber r(0);
  
  for (int i = 0; i < k.getNumObjects(); i++)
  {
    int sel = randNum(0, 2);
    if (sel == 1 && k.getCost() + k.getCost(i) <= k.getCostLimit())
    {
      k.select(i);
    }
  }
}

void generateOneNeighbor(knapsack &k, int i, int j, vector <knapsack> &neighbors)
// Generates a neighbor to k by changing the 'selected' status of items i and j based on their current values
//  1. If both items are unselected: selects one or both if they fit, or
//  2. If both items are selected: create knapsacks where i is unselected, j is unselected, and both are unselected, adds them all to a vector
//  3. If only one item is selected: flips selected and unselected if the new item fits, adds both if possible
{
  knapsack neighbor = k;
  if (neighbor.isSelected(i) == neighbor.isSelected(j) == 1)
  // If both items are selected, unselect one or both items
  {
    neighbor.unSelect(i); // Without i
    if (neighbor.getCost() <= neighbor.getCostLimit())
      neighbors.push_back(neighbor);
    neighbor.unSelect(j); // Without i or j
    if (neighbor.getCost() <= neighbor.getCostLimit())
      neighbors.push_back(neighbor);
    neighbor.select(i); // Without j
    if (neighbor.getCost() <= neighbor.getCostLimit())
      neighbors.push_back(neighbor);
  }

  else if (neighbor.isSelected(i) == neighbor.isSelected(j) == 0)
  // If neither item is selected, select one or both if possible
  {
    if (neighbor.getCost() + neighbor.getCost(i) <= neighbor.getCostLimit())
    // Try selecting object i
    {
      neighbor.select(i);
      if (neighbor.getCost() + neighbor.getCost(j) <= neighbor.getCostLimit())
      // Try to select both objects
      {
        neighbor.select(j);
      } 
    }
    else if (neighbor.getCost() + neighbor.getCost(j) <= neighbor.getCostLimit())
    // If object i doesn't fit, try object j
    {
      neighbor.select(j);
    }
    if (neighbor.getCost() <= neighbor.getCostLimit())
    // If solutions are valid, add them to the vector 'neighbors'
      neighbors.push_back(neighbor);
  }

  else if (neighbor.isSelected(i) != neighbor.isSelected(j))
  // If one item is selected and the other one isn't, flip their bits
  {
    if (neighbor.isSelected(i))
    // If i is selected
    {
      if (neighbor.getCost() + neighbor.getCost(j) <= neighbor.getCostLimit())
      // If j can fit, add j, else remove i
        neighbor.select(j);
      else neighbor.unSelect(i);

      if (neighbor.getCost() + neighbor.getCost(j) <= neighbor.getCostLimit())
      // If j can fit, add j
      {
        neighbor.select(j);
      }
    }
    else
    // If j is selected 
    {
      if (neighbor.getCost() + neighbor.getCost(i) <= neighbor.getCostLimit())
      // If i can fit, add i, else remove j
        neighbor.select(i);
      else neighbor.unSelect(j);

      if (neighbor.getCost() + neighbor.getCost(i) <= neighbor.getCostLimit())
      // If i can fit, add i
      {
        neighbor.select(i);
      }
    }
    if (neighbor.getCost() <= neighbor.getCostLimit())
    // If solutions are valid, ad them to the vector neighbors
      neighbors.push_back(neighbor);
  }
}

knapsack generateRandomNeighbor(knapsack &k)
// Generates a random neighbor by generating random indices i and j, and calling function 
// generateOneNeighbor using those indices. Stores one or three solutions in vector neighbors,
// returns one at random.
{
  knapsack neighbor(k);
  vector <knapsack> neighbors;
  int size = k.getNumObjects();
  int i = randNum(0, size);
  int j = randNum(0, size);

  generateOneNeighbor(k, i, j, neighbors);
  if (neighbors.size() != 1)
  {
    i = randNum(0, neighbors.size());
    neighbor = neighbors.at(i);
  }
  else neighbor = neighbors.at(0);
  
  return neighbor;
}

double acceptanceProb(double &oldVal, double &newVal, double &T)
// Calculates probability of acceptance
{
  //cout << "newVal is "<< newVal <<", oldVal is " << oldVal << endl;
  double e = (newVal-oldVal)/T;
  //cout << "exp is " << e << endl;
  double prob = exp(e);
  //cout << "prob is "<< prob << endl;
  if (prob >= 1) return 1;
  //if (prob == 1) return 0; 
  else return prob; 
}

int randNum(int min, int max)
// Return a random number on the interval [min, max)
{
  int x = rand() % max + min;
  return x;
}

void writeToCompare(knapsack &greedy, knapsack &steep, knapsack &anneal)
{
  ofstream outFile;
  outFile.open("output/compare.txt", ios::out | ios::app);
  outFile << "  " << greedy.getNumObjects() << "    ||	" << greedy.getValue() << "      ||	      " << steep.getValue() << "	||        " << anneal.getValue() << endl;
  outFile << "-----------------------------------------------------------------------" << endl;
}
