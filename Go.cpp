#include<bits/stdc++.h>
#include <locale>
#include <codecvt>
#define MAXN 10000
using namespace std;

double arr[MAXN];
string name[MAXN];


int main()
{
	ios::sync_with_stdio(false);
	srand(chrono::steady_clock::now().time_since_epoch().count());

	ifstream fin("Probability.txt",ios::in);
	double tmp;
	for(int i=0;fin>>tmp;i++)
		arr[i]=tmp;
	fin.close();

	fin = ifstream("RestaurantNames.txt",ios::in);
	string si;
	for (int i = 0; fin >> si; i++) {
		// cout << "READ: " << si << endl;
		name[i] = si;
	}

	fin.close();

	
	long long hh[MAXN], sum = 0;
	for (int i = 0; i < MAXN; i++) 
	{
		hh[i] = sum+arr[i]*100000;
		sum=hh[i];
	}
	
	long long min = 0;
	long long max = sum;

	long long x = (max - min) * rand() / (RAND_MAX + 1.0) + min;
	for (int i = 0; i < MAXN; i++)
	{
		if (x<=hh[i])
		{	
			cout << "Today's Lunch: " << name[i] << '\n';
			// cout << "Index: " << i << "\n";
			// cout << "Probability: " << arr[i] << '\n';
			break; 
		}
	}
	
	
	return 0;	
}
