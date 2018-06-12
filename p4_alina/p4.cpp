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

void branchAndBound(knapsack &k);

struct greaterThanByBound
{
  bool operator() (const knapsack &k1, const knapsack &k2) const
  {
    return k1.getBound() > k2.getBound();
  }
};

int main()
{
   char x;
   ifstream fin;
   stack <int> moves;
   string fileName;
   vector <knapsack> pos;
   
   // Read the name of the graph from the keyboard or
   // hard code it here for testing.
   
   fileName = "knapsack/knapsack8.input";

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

      //cout << endl << "Best solution" << endl;
      //k.printSolution();
      
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
  while (k.num)

}
