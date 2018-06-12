// Project 2a: Solving knapsack using greedy algorithm
// Alina Rossi-Conaway & Alexander Duffy

#include <iostream>
#include <limits.h>
#include <list>
#include <fstream>
#include <deque>
#include <stack>
#include <vector>
#include <time.h>
#include <string>
#include <queue>

using namespace std;

#include "d_except.h"
#include "d_matrix.h"
#include "knapsack.h"

knapsack branchAndBound(knapsack & knap)
{
    deque<knapsack> theDeq;
    //the deque
    knapsack maxKnap = knap;
    theDeq.push_back(knap);
    while (theDeq.empty()==false)
    {
        cout<<theDeq.front().bound()<< " num checked:"<<theDeq.front().getObjChecked()<<" totalnum:"<<theDeq.front().getNumObjects()<<endl;
        if (theDeq.front().getObjChecked() == theDeq.front().getNumObjects()-1)
        //getObjChecked is "num" or what object we're on
        {
            maxKnap = theDeq.front();
            float maxBound = maxKnap.getValue();
            theDeq.pop_front();
            float tempBound = theDeq.front().bound();
            //cout<<"test1";
            while (tempBound < maxBound && theDeq.empty()==false) 
            {
                theDeq.pop_front();
                if (!theDeq.empty())
                    tempBound = theDeq.front().bound();
            }
            //cout<<"test2";
        }
        //if we get a finished solution, that becomes the max(Knap)
        //then pops it off the front and checks if the next bound is greater than the max 
        //if not pops it off and continues to do so until one is not
        if (theDeq.empty()==false)
        {
            knapsack knap1 = theDeq.front();
            knapsack knap2 = theDeq.front();
            theDeq.pop_front();
            //sets two temp knapsacks as the first one which is one with a value greater than the max
            int objectInUse = knap1.getObjChecked();
            knap1.select(objectInUse);
            knap1.setObjChecked(objectInUse+1);
            knap2.unSelect(objectInUse);
            knap2.setObjChecked(objectInUse+1);
            if (reset)
                knap1=knap2;
            if (knap1.bound() > knap2.bound())
            {
                theDeq.push_front(knap1);
                theDeq.push_back(knap2);
            }
            else if (knap1.bound() < knap2.bound())
            {
                theDeq.push_front(knap2);
                theDeq.push_back(knap1);
            }
            else 
                theDeq.push_front(knap1);
        }
        

    }
    return maxKnap;
}

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

      float theBound = k.bound();
      cout << endl << "The Bound is " << theBound<< endl;
      knapsack best = branchAndBound(k);
      cout<<"The best is:"<<endl;
      best.printSolution();
      
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

