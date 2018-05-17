// Project 1a: Solving knapsack using exhaustive search
//

#include <iostream>
#include <limits.h>
#include <list>
#include <fstream>
#include <queue>
#include <stack>
#include <vector>
#include <time.h>

using namespace std;

#include "d_except.h"
#include "d_matrix.h"
#include "knapsack.h"

void exhaustiveKnapsack(knapsack &k, const int &t);
void generateAllCombinations(knapsack &k, int start, int &end, int index, vector <int> &items, vector <int> &combs, const int &t);
void generateRcombinations(knapsack &k, int &r, int start, int &end, int index, vector <int> &items, vector <int> &comb);


int main()
{
   char x;
   ifstream fin;
   stack <int> moves;
   string fileName;
   vector <knapsack> pos;
   
   // Read the name of the graph from the keyboard or
   // hard code it here for testing.
   
   fileName = "knapsack/knapsack32.input";

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

      exhaustiveKnapsack(k, 600);

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

void exhaustiveKnapsack(knapsack &k, const int &t)
{
    int n = k.getNumObjects() - 1;

    vector <int> items;
    for (int i = 0; i <= n; i++)
    {
        items.push_back(i);
    }

    vector <int> comb;

    generateAllCombinations(k, 0, n, 0, items, comb, t);
}

void generateAllCombinations(knapsack &k, int start, int &end, int index, vector <int> &items, vector <int> &combs, const int &t)
// Calls generateRCombinations in a for loop to generate all combinations from size n (number of items) down to 1
{
    clock_t startTime = clock();

    for (int r = end+1; r > 0; r--)
    {
        combs.clear();
        combs.resize(r);

        int diff = clock() - startTime;
        int runTime = diff / CLOCKS_PER_SEC;
        cout << "runtime is " << runTime << endl;

        if (runTime <= t)
        {    
            generateRcombinations(k, r, start, end, index, items, combs);
        }
        else return;
    }
}

void generateRcombinations(knapsack &k, int &r, int start, int &end, int index, vector <int> &items, vector <int> &comb)
/* Recursive function to generate all combinations of size r of n items, which are stored in the items vector. 
These combinations are stored in the vector comb, one at a time, and the value and cost of each combination is evaluated. 
Value and cost of k, the optimal knapsack, is updated accordingly.
 */
{
    if (index == r) // Combination of size r is ready to be evaluated
    {
        int cost = 0;
        int value = 0;

        for (int i = 0; i < r; i++) // Evaluates cost of combination
        {
            cost = cost + k.getCost(comb.at(i));
        }
        for (int i = 0; i < r; i++) // Evaluates value of combination
        {
            value = value + k.getValue(comb.at(i));
        }
        if (cost <= k.getCostLimit() && value > k.getValue()) // If new knapsack has higher value and meets cost restraint
        {
            k.unSelectAll();

            for (int i = 0; i < r; i++) // Select combination of items
            {
                k.select(comb.at(i));
            }
        }
        return;
    }

    for (int i = start; i <= end && end-i+1 >= r-index; i++) // Triggers recursion
    {
        comb.at(index) = items.at(i);
        generateRcombinations(k, r, i+1, end, index+1, items, comb);
    }        
}

