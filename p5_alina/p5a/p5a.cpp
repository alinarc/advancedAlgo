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

void greedyKnapsack(knapsack &k);
void generateNeighbors(knapsack &k);
void generateRandomSolution(knapsack &k);

int main()
{
   char x;
   ifstream fin;
   stack <int> moves;
   string fileName;
   vector <knapsack> pos;
   
   // Read the name of the graph from the keyboard or
   // hard code it here for testing.
   
   fileName = "knapsack/knapsack16.input";

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

      knapsack curr;
      while (curr.getValue() != k.getValue())
      {
        curr = k;
        generateNeighbors(k);
      }
      cout << endl << "Steepest descent solution:" << endl;
      k.printSolution(); */

      generateRandomSolution(k);
      cout << endl << "Randomly generated solution: " << endl;
      k.printSolution();
      
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
// Greedy algorithm to find optimal knapsack solution. Considers item in decreasing order of ratio (of value to cost), adds them if they don't create a conflict (exceed costLimit).
{
    int i = 0;
    while (k.getCost() + k.getCost(k.getVector(i).at(0)) < k.getCostLimit() && i < k.getNumObjects())
    {
        k.select(k.getVector(i).at(0));
        i+=1;
    }
}

void generateNeighbors(knapsack &k)
{
  cout << "exploring neighbors " << endl;
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

void generateRandomSolution(knapsack &k)
{
  randomNumber r(0);
  /* int max = pow(2, (k.getNumObjects() - 1)) - 1;
  cout << "max is " << max << endl;
  int num = r.random(max);
  cout << "random number is " << num << endl;
  int binary[k.getNumObjects()];
  for (int i = 0; i <= k.getNumObjects(); i++)
  {
    binary[i] = (num >> i) & 1;
    if (binary[i] == i) k.select(i);
  } */

  for (int i = 0; i < k.getNumObjects(); i++)
  {
    int sel = r.random(2);
    if (sel == 1 && k.getCost() + k.getCost(i) <= k.getCostLimit())
    {
      k.select(i);
    }
  }
}
