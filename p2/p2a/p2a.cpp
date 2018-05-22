// Project 1a: Solving knapsack using exhaustive search
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

void greedyKnapsack(knapsack &k, const int &t);

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

      greedyKnapsack(k, 600);

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

void greedyKnapsack(knapsack &k, const int &t)
{
    clock_t startTime = clock();
    int i = 0;
    while (k.getCost()+k.getCost(k.getVector(i).at(0)) < k.getCostLimit() && i < k.getNumObjects())
    {
        int diff = clock() - startTime;
        int runTime = diff / CLOCKS_PER_SEC;

        if (runTime <= t)
        {
            //cout << "evaluating item " << k.getVector(i).at(0) << endl;
            k.select(k.getVector(i).at(0));
            i+=1;
        }
        else
        {
            cout << "time is " << runTime << endl;
            break;
        }
    }
}