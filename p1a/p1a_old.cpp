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
void generateAlternatives(knapsack &k, int curr, vector <knapsack> &pos);

int main()
{
   char x;
   ifstream fin;
   stack <int> moves;
   string fileName;
   vector <knapsack> pos;
   
   // Read the name of the graph from the keyboard or
   // hard code it here for testing.
   
   fileName = "knapsack/knapsack28.input";

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
    int curr = k.getNumObjects() - 1;
//    int limit = k.getCostLimit() - k.getCost();
    vector <knapsack> pos;
    pos.push_back(k);
    if (curr >= 0)
    {
        if (k.getCost(curr) > k.getCostLimit())
        {
            k.unSelect(curr);
            //exhaustiveKnapsack(k, t);
        }
        else
        {
            generateAlternatives(k, curr, pos);
        }

        //curr = curr - 1;
        //cout << "current item is now " << curr << endl;
    }  
    int max = 0 , imax = 0;
    for (int i = 0; i < pos.size(); i++)
    {
        if (pos.at(i).getValue() > max && pos.at(i).getCost() <= pos.at(i).getCostLimit())
        {
            max = pos.at(i).getValue();
            imax = i;
        }
    }

    k = pos.at(imax);
}

void generateAlternatives(knapsack &k, int curr, vector <knapsack> &pos)
{
    knapsack k1(k);
    knapsack k2(k);

    if (k1.getCost() + k1.getCost(curr) <= k1.getCostLimit())
    {
        k1.select(curr);
        pos.push_back(k1);
        /* cout << "working with knapsack in location " << pos.size()-1 <<  " in vector" << endl;
         */if (curr > 0)
            generateAlternatives(pos.at(pos.size()-1), curr-1, pos);
    }
    k2.unSelect(curr);
        pos.push_back(k2);
        /* cout << "working with knapsack in location " << pos.size() -1 << " in vector " << endl;
         */if (curr > 0)
            generateAlternatives(pos.at(pos.size()-1), curr-1, pos);
        
}

