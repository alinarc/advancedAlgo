// Project 4: Solving knapsack using branch-and-bound
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

using namespace std;

#include "d_except.h"
#include "d_matrix.h"
#include "knapsack.h"

struct greaterThanByBound
{
  bool operator() (const knapsack &k1, const knapsack &k2) const
  {
    return k1.getBound() > k2.getBound();
  }
};

void branchAndBound(knapsack &k, int t);
void exploreBranch(knapsack &currK, knapsack &maxK, priority_queue <knapsack, vector <knapsack>, greaterThanByBound> &pq);

int main()
{
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

      branchAndBound(k, 600);

      cout << endl << "Best solution" << endl;
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

void branchAndBound(knapsack &k, int t)
// Branch-and-bound algorithm to find optimal knapsack solution. Calls function exloreBranch inside the main loop 
// and considers all knapsacks in the priority queue, in decreasing order of bound value.
{
  clock_t startTime = clock();
  int runTime;

  priority_queue <knapsack, vector <knapsack>, greaterThanByBound> pq;
  pq.push(k);
  knapsack currK(k);
  knapsack maxK = currK;
  exploreBranch(currK, maxK, pq);

  while (!pq.empty())
  // Evaluates every knapsack in the priority queue
  {
    
    clock_t curr = clock();
    int diff = curr - startTime;
    runTime = diff / CLOCKS_PER_SEC;
    if (runTime <= t)
    // Times the program based on time limit t
    {
      currK = pq.top();
      pq.pop();
      if (currK.getBound() > maxK.getValue())
      // Explores branch of knapsack currK if its bound is higher than the best knapsack found so far.
      {
        exploreBranch(currK, maxK, pq);
        // Stores knew best knapsack (stored in currK) from exploring this branch, compares it to best so far.
        if (currK.getValue() > maxK.getValue())
          maxK = currK;
      }
    }
    else break; // Breaks while loop if time limit is reached.
  }
  cout << "runTime is " << runTime << endl;
  k = maxK;
}

void exploreBranch(knapsack &currK, knapsack &maxK, priority_queue <knapsack, vector <knapsack>, greaterThanByBound> &pq)
// Helper function that explores branch of (considers all objects for) knapsack currK, and stores the best found in that branch in knapsack maxK
// Makes copy knapsacks along the way, makes decisions for them, and stores them in pq to be explored later
{  
  while (currK.getNum() < currK.getNumObjects() - 1) // Explores all depth levels (considers all objects) to return a complete solution in knapsack currK
  {
    int num = currK.getNum() + 1; // Increments num (level)
    currK.setNum(num);
    int item = currK.getMatrix(num, 0);
    
    knapsack k1 (currK); // Copies of current knapsack
    knapsack k2 (currK);
    if (k1.getCost() + k1.getCost(item) <= k1.getCostLimit()) // Checks if selecting k makes a valid solution
    {
      k1.select(item);
    }
    else if (k1.getValue() > maxK.getValue()) // Stores max knapsack found
    {
      maxK = k1;
    }

    if (k2.bound() > maxK.getValue()) // If bound is higher than max so far
    {
      pq.push(k2); // Add it to the queue to be explored later
    }
    currK = k1;
  }
}
