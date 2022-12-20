#include <bits/stdc++.h>
#include "LinearProgramming.h"
using namespace std;


// const double EPS = 1e-9;
// const double INF = 1e9;

int SZ; // # of type of foods

vector<vector<double> > data;
vector<double> Min, Max;
vector<double> data_query;

vector<string> str_query = {"Distance?", "Shop Rating?", "Calories?", "Price?", "Food Rating?"};


int main(){
	
	// read data from Data2.txt
	freopen("Data.txt", "r", stdin);

	data.resize((int)str_query.size() + 1);
	Min.assign((int)str_query.size(), INF);
	Max.assign((int)str_query.size(), -INF);
	data_query.resize((int)str_query.size());

	SZ = 0;
	for(int type = 0; type < (int)str_query.size(); type++){
		string line; getline(cin, line);
		stringstream ss(line);
		double x;
		while (ss >> x) {
			if (type == 0) SZ++;
			data[type].emplace_back(x);
			Min[type] = min(Min[type], x);
			Max[type] = max(Max[type], x);
		}
	}
	freopen("CON", "r", stdin);
	
	// check if input is valid: min <= input <= max
	for(int type = 0; type < (int)str_query.size(); type++){
		cout<<str_query[type]<<" ("<<Min[type]<<" ~ "<<Max[type]<<"): ";
		do{
			cin>>data_query[type];
		}while(Min[type] > data_query[type] or Max[type] < data_query[type]);
	}

	/* ========================================================================= */

	freopen("Probability.txt", "w", stdout);

	int epsDigit = 9;
	cerr << fixed << setprecision(epsDigit);
	cout << fixed << setprecision(epsDigit);


	// sum of probability must = 1

	for (int xi = 0; xi < SZ; xi++)
		data.back().emplace_back(1.0);

	data_query.emplace_back(1.0);



	// model for linear programming

	vector<vector<double>> A;
	vector<double> B, C;

	int nVar = SZ;
	int nCon = (int)data.size() * 2;

	A.clear(); A.resize(nCon, vector<double>(nVar, 0));
	B.assign(nCon, 0);
	C.assign(nVar, 0);

	for (int i = 0, j = 0; i < (int)data.size(); i++, j += 2) {
		for (int xi = 0; xi < nVar; xi++) {
			A[j][xi] = data[i][xi];
			A[j+1][xi] = -1 * data[i][xi];
		}
		B[j] = data_query[i];
		B[j+1] = -1 * data_query[i];
	}


	LinearProgramming LP_noMinProbConstraint;
	LP_noMinProbConstraint.inputConstraints(A, B, C);

	vector<double> bfs;
	bool result = LP_noMinProbConstraint.initBFS(bfs);
	if (!result) {
		double P = 1.0 / nVar;
		cerr << "No Feasible Solution.\n";
		for (int xi = 0; xi < nVar; xi++)
			cout << P << ' ';
		cout << '\n';
		return 0;
	}

	vector<double> noMinConstraintP;
	for (int xi = 0; xi < nVar; xi++)
		noMinConstraintP.emplace_back(bfs[xi]);


	nCon += nVar;
	A.resize(nCon, vector<double>(nVar, 0));
	B.resize(nCon, 0);
	for (int xi = 0, i = (int)data.size() * 2; xi < nVar; xi++, i++)
		A[i][xi] = -1;

	LinearProgramming LP;
	function<bool(double)> check = [&](double p) -> bool {
		// check if feasible when all xi must >= p
		for (int xi = 0, i = (int)data.size() * 2; xi < nVar; xi++, i++)
			B[i] = -p;

		LP.inputConstraints(A, B, C);
		bool result = LP.initBFS(bfs);

		return result;
	};

	double l = 0.0, r = 1.0;
	for (int time = 0; time < 40; time++) {
		// cerr << "Binary Search: " << l << ' ' << r << endl;
		double mid = (l+r) / 2;
		bool ok = check(mid);
		if (ok) {
			cerr << "OK: " << mid << endl;
			l = mid;
		}
		else r = mid;
	}

	if (!check(l)) {
		cerr << "Only Feasible When No Constraints.\n";
		for (int xi = 0; xi < nVar; xi++)
			cout << noMinConstraintP[xi] << ' ';
		cout << '\n';
	}
	else {
		cerr << "Success! min(P) = " << l << '\n';
		for (int xi = 0; xi < nVar; xi++)
			cout << bfs[xi] << ' ';
		cout << '\n';
	}



	/* ========================================================================== */
	
	return 0;
}
/* TEST DATA



700
80
800
77
8




300
77
900
70
8

700
80
900
80
8

200
80
900
80
8

400
76
800
100
8


*/