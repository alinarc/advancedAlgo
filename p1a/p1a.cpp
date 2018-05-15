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
void generateRcombinations(knapsack &k, int &r, int start, int &end, int index, vector <int> &items, vector <int> &comb);
void generateAllCombinations(knapsack &k, int start, int &end, int index, vector <int> &items, vector <int> &combs);

int main()
{
   char x;
   ifstream fin;
   stack <int> moves;
   string fileName;
   vector <knapsack> pos;
   
   // Read the name of the graph from the keyboard or
   // hard code it here for testing.
   
   fileName = "knapsack/knapsack48.input";

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

      //cout << "the total value of the knapsack is " << k.getValue() << endl;

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
    int r = 2;

    vector <int> items;
    for (int i = 0; i <= n; i++)
    {
        items.push_back(i);
    }

    vector <int> comb;

    generateAllCombinations(k, 0, n, 0, items, comb);

  /*   if (curr >= 0)
    {
        if (k.getCost(curr) > k.getCostLimit())
        {
            k.unSelect(curr);
            //exhaustiveKnapsack(k, t);
        }
        else
        {
            generateAlternatives(k, curr, pos);
        } */

}

void generateAllCombinations(knapsack &k, int start, int &end, int index, vector <int> &items, vector <int> &combs)
{
    cout << "generating all combinations " << endl;
    for (int r = end+1; r > 0; r--)
    {
        combs.clear();
        combs.resize(r);
        generateRcombinations(k, r, start, end, index, items, combs);
    }
}

void generateRcombinations(knapsack &k, int &r, int start, int &end, int index, vector <int> &items, vector <int> &comb)
{
    //cout << "generating combinations of size " << r << endl;
    if (index == r)
    {

        int cost = 0;
        int value = 0;

        for (int i = 0; i < r; i++)
        {
            cost = cost + k.getCost(comb.at(i));
        }
        for (int i = 0; i < r; i++)
        {
            value = value + k.getValue(comb.at(i));
        }
        if (cost <= k.getCostLimit() && value > k.getValue())
        {
            for (int i = 0; i <= end; i++)
            {
                k.unSelect(i);
            }
            for (int i = 0; i < r; i++)
            {
                k.select(comb.at(i));
            }
        }
        return;
    }

    for (int i = start; i <= end && end-i+1 >= r-index; i++)
    {
        comb.at(index) = items.at(i);
        generateRcombinations(k, r, i+1, end, index+1, items, comb);
    }        
}

