#include <bits/stdc++.h>
#include "Matrix.h"
#include "LinearProgramming.h"
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


LinearProgramming::LinearProgramming() {
	numConstraint = 0;
	numVar = 0;
	pivotVar.clear();
	mtx = Matrix();
}
LinearProgramming::LinearProgramming(int _numVar, int _numConstraint) {
	numConstraint = _numConstraint;
	numVar = _numVar;
	pivotVar.resize(_numConstraint + 1, -1);
	mtx = Matrix(_numConstraint + 1, _numVar + _numConstraint + 1);
}

bool LinearProgramming::isEqual(double a, double b) {
	return abs(a-b) <= EPS;
}

void LinearProgramming::inputConstraints(vector<vector<double> >& A, vector<double>& B, vector<double>& C) {
	// max Cx
	// subject to Ax <= B, x >= 0

	// normal var: x[1] ~ x[nVar] (1 base!!!)
	//  slack var: x[nVar+1] ~ x[nVar+nCon]
	//    aux var: x[nVar+nCon+1]

	numVar = (int)C.size();
	numConstraint = (int)B.size();
	assert((int)A.size() == numConstraint);
	assert((int)A[0].size() == numVar);
	pivotVar.assign(numConstraint + 1, -1);
	mtx = Matrix(numConstraint + 1, numVar + numConstraint + 1);

	int slack = numVar + 1;
	for (int col = 1; col <= numVar; col++) {
		double c = C[col-1];
		mtx.setEntry(0, col, c);
	}
	for (int row = 1; row <= numConstraint; row++) {
		for (int col = 1; col <= numVar; col++) {
			double a = A[row-1][col-1];
			mtx.setEntry(row, col, a);
		}
		double b = B[row-1];
		mtx.setEntry(row, 0, b);
		mtx.setEntry(row, slack, 1);
		pivotVar[row] = slack;
		slack++;
	}
}

void LinearProgramming::pivot(int row, int col) {
// cerr << "PIVOT " << row << ' ' << col << '\n';
	pivotVar[row] = col;
	mtx.MultiplyRow(row, 1.0/mtx.getEntry(row, col));
	for (int ri = 0; ri < mtx.getRowSize(); ri++) {
		if (isEqual(0, mtx.getEntry(ri, col)) || ri == row) continue;
		mtx.AddRowToAnother(row, -mtx.getEntry(ri, col), ri);
	}
}

bool LinearProgramming::initBFS(vector<double>& bfs) {
	// find init basic feasible solution

	mt19937 gen(chrono::steady_clock::now().time_since_epoch().count());
	uniform_int_distribution<int> dis(1, 1e9);

	int pivCnt = 0;

	while (true) {

		double minB = -EPS;
		int whichRowHasMinB = 0;
		for (int row = 1; row <= numConstraint; row++) {
			double b = mtx.getEntry(row, 0);
			if (b < minB) {
				minB = b;
				whichRowHasMinB = row;
			}
		}
		if (whichRowHasMinB == 0) break;

		int whichVarHasNegA = 0;
		for (int var = 1; var <= numVar + numConstraint; var++) {
			double a = mtx.getEntry(whichRowHasMinB, var);
			if (a < -EPS && (!whichVarHasNegA || (dis(gen) & 1))) {
				whichVarHasNegA = var;
				break;
			}
		}

		if (whichVarHasNegA == 0) {
			// Infeasible
			return false;
		}

		pivot(whichRowHasMinB, whichVarHasNegA);

		pivCnt++;
		if (pivCnt > 3000) return false;
	}

	bfs.clear(); bfs.resize(numVar + numConstraint, 0);  // 1-base to 0-base
	for (int row = 1; row <= numConstraint; row++) {
		bfs[pivotVar[row]-1] = mtx.getEntry(row, 0);
	}

	return true;
}


int LinearProgramming::solve(vector<double>& ans, double& mxObj) {
	// -1: no solution, 0: unbounded, 1: OK

	vector<double> bfs;
	bool isFeasible = initBFS(bfs);
	if (!isFeasible) return -1;

	while (true) {
		// check if optimal, that is, c[i] <= 0 forall i
		double maxC = 0;
		int whichVarHasMaxC = 0;
		bool isOptimal = true;
		for (int var = 1; var <= numVar + numConstraint; var++) {
			double c = mtx.getEntry(0, var);
			if (c > EPS) {
				isOptimal = false;
				if (c > maxC) {
					maxC = c;
					whichVarHasMaxC = var;
				}
			}
		}
		if (isOptimal) break;

		double minBdivA = INF;
		int whichRowHasMinBdivA = 0;
		for (int row = 1; row <= numConstraint; row++) {
			double a = mtx.getEntry(row, whichVarHasMaxC);
			double b = mtx.getEntry(row, 0);
			if (a > EPS && b/a < minBdivA) {
				minBdivA = b/a;
				whichRowHasMinBdivA = row;
			}
		}

		if (whichRowHasMinBdivA == 0) {
			return 0;
		}

		pivot(whichRowHasMinBdivA, whichVarHasMaxC);
	}

	ans.clear(); ans.resize(numVar + numConstraint, 0);  // 1-base to 0-base
	for (int row = 1; row <= numConstraint; row++) {
		ans[pivotVar[row]-1] = mtx.getEntry(row, 0);
	}

	mxObj = -1 * mtx.getEntry(0, 0);

	return 1;
}

void LinearProgramming::setOutputPrecision(int p) {
	mtx.setOutputPrecision(p);
}

