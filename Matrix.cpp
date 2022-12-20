#include <iostream>
#include <iomanip>
#include <vector>
#include <math.h>
#include <assert.h>
#include "Matrix.h"
using namespace std;

template<typename T>
ostream& operator<<(ostream& os, const vector<T>& vec) {
	for (int i = 0; i < (int)vec.size(); i++)
		os << vec[i] << ' ';
	return os << endl;
}

template<typename T>
ostream& operator<<(ostream& os, const vector<vector<T>>& vec) {
	for (int i = 0; i < (int)vec.size(); i++) {
		for (int j = 0; j < (int)vec[0].size(); j++)
			os << vec[i][j] << ' ';
		os << endl;
	}
	return os << endl;
}

#define LOG(var) cout << "=== " << #var << ": " << var << " ===" << endl;




Matrix::Matrix() {
	sizeRow = 0;
	sizeColumn = 0;
	matrix.clear();
}

Matrix::Matrix(int _sizeRow, int _sizeColumn) {
	sizeRow = _sizeRow;
	sizeColumn = _sizeColumn;
	matrix.clear();
	matrix.resize(sizeRow, vector<double>(sizeColumn, 0));
}

bool Matrix::isEqual(double a, double b) {
	return abs(a-b) <= EPS;
}

int Matrix::getRowSize() { return sizeRow; }
int Matrix::getColumnSize() { return sizeColumn; }

void Matrix::ResetMatrix(int newSizeRow, int newSizeColumn) {
	if (newSizeRow < 0 || newSizeColumn < 0) return;
	sizeRow = newSizeRow;
	sizeColumn = newSizeColumn;
	matrix.clear();
	matrix.resize(sizeRow, vector<double>(sizeColumn, 0));
}

void Matrix::setEntry(int row, int column, double value) {
	// row, column are 0-based
	if (row < 0 || row >= sizeRow) return;
	if (column < 0 || column >= sizeColumn) return;
	matrix[row][column] = value;
}

double Matrix::getEntry(int row, int column) {
	// row, column are 0-based
	if (row < 0 || row >= sizeRow) return 0;
	if (column < 0 || column >= sizeColumn) return 0;
	return matrix[row][column];
}


// Gaussian-Jordan Elimination

void Matrix::SwapRow(int row1, int row2) {
	if (row1 < 0 || row1 >= sizeRow) return;
	if (row2 < 0 || row2 >= sizeRow) return;
	swap(matrix[row1], matrix[row2]);
}

void Matrix::MultiplyRow(int row, double value, int startCol) {
	if (row < 0 || row >= sizeRow) return;
	for (int i = startCol; i < sizeColumn; i++) {
		matrix[row][i] *= value;
	}
}

void Matrix::AddRowToAnother(int rowFrom, double value, int rowTo, int startCol) {
	if (rowFrom < 0 || rowFrom >= sizeRow) return;
	if (rowTo < 0 || rowTo >= sizeRow) return;
	for (int i = startCol; i < sizeColumn; i++) {
		matrix[rowTo][i] += matrix[rowFrom][i] * value;
	}
}

void Matrix::GaussJordanElimination() {
	int row = 0;
	for (int col = 0; col < sizeColumn; col++) {

		if (row >= sizeRow) break;

		bool hasPivot = false;
		if (!isEqual(matrix[row][col], 0)) hasPivot = true;
		else for (int i = row+1; i < sizeRow; i++) {
			if (isEqual(matrix[i][col], 0)) continue;
			SwapRow(row, i);
			hasPivot = true;
			break;
		}
		if (!hasPivot) continue;

		MultiplyRow(row, 1.0/matrix[row][col], col+1);
		matrix[row][col] = 1;

		for (int i = 0; i < sizeRow; i++) {
			if (i == row) continue;
			AddRowToAnother(row, -matrix[i][col], i, col);
		}

		row++;
	}
}


// Print Matrix

void Matrix::setOutputPrecision(int p) {
	if (p < 0) return;
	cout << fixed << setprecision(p);
}

void Matrix::printMatrix() {
	cout << "===== Matrix =====\n" << matrix;
}


// int main() {

// 	int szRow, szCol;
// 	cin >> szRow >> szCol;

// 	Matrix mtx(szRow, szCol);
// 	mtx.setOutputPrecision(0);

// 	for (int i = 0; i < szRow; i++) {
// 		for (int j = 0; j < szCol; j++) {
// 			double x; cin >> x;
// 			mtx.setEntry(i, j, x);
// 		}
// 	}

// 	mtx.GaussJordanElimination();
// 	mtx.printMatrix();

// 	return 0;
// }
/*

2 5
5 4 8 3 4
6 7 3 9 7

2 4
1 1 1 3
1 2 -1 4

3 4
1 2 3 5
2 4 8 12
3 6 7 13

3 5
1 2 1 0 4
2 4 4 8 2
4 8 6 8 10



*/