#include <bits/stdc++.h>
#include "Matrix.h"
using namespace std;
#pragma once

class LinearProgramming {

private:

	Matrix mtx;
	int numConstraint, numVar;
	vector<int> pivotVar; // pivot var for each constraint (each row)

	bool isEqual(double a, double b);

public:
	
	LinearProgramming();
	LinearProgramming(int _numVar, int _numConstraint);
	~LinearProgramming() {}

	void inputConstraints(vector<vector<double> >& A, vector<double>& B, vector<double>& C);

	void pivot(int row, int col);

	bool initBFS(vector<double>& bfs);

	int solve(vector<double>& ans, double& mxObj);

	void setOutputPrecision(int p);

};