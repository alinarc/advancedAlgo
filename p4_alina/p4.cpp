// Project 2a: Solving knapsack using greedy algorithm
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

void branchAndBound(knapsack &k);
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

      //greedyKnapsack(k);


      //cout << "Optimistic bound for value: " << k.bound() << endl;

      branchAndBound(k);

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

void branchAndBound(knapsack &k)
// Greedy algorithm to find optimal knapsack solution. Considers item in decreasing order of ratio (of value to cost), adds them if they don't create a conflict (exceed costLimit).
{
  priority_queue <knapsack, vector <knapsack>, greaterThanByBound> pq;
  pq.push(k);
  knapsack currK(k);
  knapsack maxK = currK;
  exploreBranch(currK, maxK, pq);
  //maxK = currK;

  /* cout << "bound on currK is " << currK.getBound() << endl;
  cout << "value of maxK is " << maxK.getValue() << endl;
  cout << "pq has " << pq.size() << " items" << endl; */
  while (!pq.empty())
  {
    currK = pq.top();
    //cout << "bound on currK is " << currK.getBound() << endl;
    pq.pop();
    if (currK.getBound() > maxK.getValue())
    {
      //cout << "exploring branch with bound " << currK.getBound() << endl;
      exploreBranch(currK, maxK, pq);
      if (currK.getValue() > maxK.getValue())
        maxK = currK;
    }
  }
  k = maxK;
}

void exploreBranch(knapsack &currK, knapsack &maxK, priority_queue <knapsack, vector <knapsack>, greaterThanByBound> &pq)
{ 
  //cout << "exploring branch for knapsack " << currK.getNum() << endl; 
  while (currK.getNum() < currK.getNumObjects() - 1)
  {
    int num = currK.getNum() + 1;
    currK.setNum(num);
    int item = currK.getMatrix(num, 0);
    
    knapsack k1 (currK);
    knapsack k2 (currK);
    if (k1.getCost() + k1.getCost(item) <= k1.getCostLimit())
    {
      k1.select(item);
    }
    else if (k1.getValue() > maxK.getValue())
    {
      maxK = k1;
    }

/*     if (k1.bound() > maxK.getValue())
    {
      pq.push(k1);
    } */
    if (k2.bound() > maxK.getValue())
    {
      pq.push(k2);
    }
    currK = k1;
  }
}
