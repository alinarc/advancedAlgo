#ifndef MATRIX_CLASS
#define MATRIX_CLASS

#include <iostream>
#include <vector>

#include "d_except.h"

using namespace std;

template <typename T>
class matrix
{
	public:
		matrix(int numRows = 1, int numCols = 1, const T& initVal = T());
			// constructor.
			// Postcondition: create array having numRows x numCols elements
			// all of whose elements have value initVal

		//matrix(vector <T> index, vector <T> cost, vector <T> value, vector <T> ratio);

		void populateWithVectors(vector <T> index, vector <T> cost, vector <T> value, vector <T> ratio);

		vector<T>& operator[] (int i);
			// index operator.
			// Precondition: 0 <= i < nRows. a violation of this
			// precondition throws the indexRangeError exception.
			// Postcondition: if the operator is used on the left-hand
			// side of an assignment statement, an element of row i 
			// is changed

		const vector<T>& operator[](int i) const;
			// version for constant objects

      int rows() const;
			// return number of rows
      int cols() const;
			// return number of columns

	  void resize(int numRows, int numCols);
	  	// modify the matrix size.
			// Postcondition: the matrix has size numRows x numCols.
			// any new elements are filled with the default value of type T
	
	  void swap(int i1, int i2);

	  void sortByRatio();
			

	private:
      int nRows, nCols;
			// number of rows and columns

      vector<vector<T> > mat;
			// matrix is implemented as nRows vectors (rows),
			// each having nCols elements (columns)
};

template <typename T>
matrix<T>::matrix(int numRows, int numCols, const T& initVal):
	nRows(numRows), nCols(numCols),
	mat(numRows, vector<T>(numCols,initVal))
{}

template <typename T>
void matrix<T>::populateWithVectors(vector <T> index, vector <T> cost, vector <T> value, vector <T> ratio)
{
	resize(index.size(), 4);
	for(int i = 0; i < rows(); i++)
	{
		mat[i][0] = index.at(i);
		mat[i][1] = cost.at(i);
		mat[i][2] = value.at(i);
		mat[i][3] = ratio.at(i);
	}
}	

/* template <typename T>
matrix<T>::matrix(vector <T> index, vector <T> cost, vector <T> value, vector <T> ratio)
{
	resize(index.size(), 4);
	for(int i = 0; i < rows(); i++)
	{
		mat[i][0] = index.at(i);
		mat[i][1] = cost.at(i);
		mat[i][2] = value.at(i);
		mat[i][3] = ratio.at(i);
	}
}	 */
	
// non-constant version. provides general access to matrix
// elements
template <typename T>
vector<T>& matrix<T>::operator[] (int i)
{
	if (i < 0 || i >= nRows)
		throw indexRangeError(
			"matrix: invalid row index", i, nRows);

   return mat[i];
}

// constant version.  can be used with a constant object.
// does not allow modification of a matrix element
template <typename T>
const vector<T>& matrix<T>::operator[] (int i) const
{
	if (i < 0 || i >= nRows)
		throw indexRangeError(
			"matrix: invalid row index", i, nRows);

   return mat[i];
}

template <typename T>
int matrix<T>::rows() const
{
   return nRows;
}

template <typename T>
int matrix<T>::cols() const
{
   return nCols;
}

template <typename T>
void matrix<T>::resize(int numRows, int numCols)
{
   int i;
   
   // handle case of no size change with a return
   if (numRows == nRows && numCols == nCols)
      return;

	// assign the new matrix size
	nRows = numRows;
	nCols = numCols;

	// resize to nRows rows
	mat.resize(nRows);

	// resize each row to have nCols columns
	for (i=0; i < nRows; i++)
		mat[i].resize(nCols);
}

template <typename T>
void matrix<T>::swap(int i1, int i2)
{
	vector <T> temp = mat[i1];
	mat[i1] = mat[i2];
	mat[i2] = temp;
}

template <typename T>
void matrix<T>::sortByRatio()
{
	cout << "Data sorted by ratio of value to cost: " << endl;
	cout << "index value cost ratio" << endl;
	int i, j, iMax;

	for (i = 0; i < rows() - 1; i++)
	{

		iMax = i;
		for (j = i+1; j < rows(); j++)
		{
			if (mat[j][3] > mat[iMax][3])
				iMax = j;
		}
		swap(iMax, i);
	}
}

template <typename T>
ostream &operator<<(ostream &ostr, const matrix<T> &m)
{
	for (int i = 0; i < m.rows(); i++)
	{
		for (int j = 0; j < m.cols(); j++)
		{
			cout << m[i][j] << " ";
		}
		cout << endl;
	}
	return ostr;
}

#endif	// MATRIX_CLASS
