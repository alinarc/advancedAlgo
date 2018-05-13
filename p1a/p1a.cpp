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

      //cout << "the total value of the knapsack is " << k.getValue() << endl;

      exhaustiveKnapsack(k, 600);

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

void exhaustiveKnapsack(knapsack &k, const int &t)
{
    int curr = k.getNumObjects() - 1;
//    int limit = k.getCostLimit() - k.getCost();
    pos.push_back(k);
    while (curr >= 0)
    {
        if (k.getCost(curr) > k.getCostLimit())
        {
            k.unSelect(curr);
            //exhaustiveKnapsack(k, t);
        }
        else
        {
            knapsack k1(k); 
            knapsack k2(k);

            k1.select(curr);
                pos.push_back(k1);
                exhaustiveKnapsack(pos.at(pos.size()-1), t);
            k2.unSelect(curr);
                pos.push_back(k2);
                exhaustiveKnapsack(pos.at(pos.size()-1), t);
        }

        curr = curr - 1;
        cout << "current item is now " << curr << endl;
    }  
}