#include <iostream>
#include <iomanip>
#include <vector>
#include <math.h>
#include <assert.h>
using namespace std;
#pragma once

static const double EPS = 1e-9;
static const double INF = 1e9;


class Matrix {

private:

	int sizeRow, sizeColumn;
	vector<vector<double>> matrix;

	bool isEqual(double a, double b);

public:

	Matrix();
	Matrix(int _sizeRow, int _sizeColumn);
	~Matrix() {}

	int getRowSize();
	int getColumnSize();
	
	void ResetMatrix(int newSizeRow, int newSizeColumn);

	void setEntry(int row, int column, double value);

	double getEntry(int row, int column);


	// Gaussian-Jordan Elimination

	void SwapRow(int row1, int row2);

	void MultiplyRow(int row, double value, int startCol=0);

	void AddRowToAnother(int rowFrom, double value, int rowTo, int startCol=0);

	void GaussJordanElimination();


	// Print Matrix

	void setOutputPrecision(int p);

	void printMatrix();

};
