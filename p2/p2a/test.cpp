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

#include "d_matrix.h"

int main()
{
    vector <float> index(3), cost(3), value(3), ratio(3);
    int c[] = {1, 1, 1};
    int v[] = {10, 20, 30};

    for (int i = 0; i < index.size(); i++)
    {
        index.at(i) = i;
        cost.at(i) = c[i];
        value.at(i) = v[i];
        ratio.at(i) = v[i] / c[i];
    }

    matrix <float> m(index, cost, value, ratio);
    cout << m << endl;

    m.sortByRatio();

    cout << m << endl;
}