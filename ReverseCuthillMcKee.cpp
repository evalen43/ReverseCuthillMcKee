// ReverseCuthillMcKee.cpp : This file contains the 'main' function. Program execution begins and
// ends there. C++ program for Implementation of Reverse Cuthill Mckee Algorithm
#include <iostream>
#include <stdio.h>
#include <fstream>
#include <vector>
#include <queue>
#include <string>

using namespace std;
vector<double> globalDegree;
int ne = 0;
vector<vector<int>> edges;
int findIndex(vector<pair<int, double> > a, int x)
{
	for (int i = 0; i < a.size(); i++)
		if (a[i].first == x)
			return i;
	return -1;
}

bool compareDegree(int i, int j)
{
	return ::globalDegree[i] < ::globalDegree[j];
}

template <typename T>
ostream& operator<<(ostream& out, vector<T> const& v)
{
	for (int i = 0; i < v.size(); i++)
		out << v[i] << ' ';
	return out;
}

class ReorderingSSM 
{
	private:
		vector<vector<double> > _matrix;

	public:
	// Constructor and Destructor
	ReorderingSSM(vector<vector<double> > m, int num_rows, int ne,string filein)
	{

		_matrix = m;
		vector<int> inc;
		vector<vector<int>> edges;
		int j1=0, j2=0;

		

		for (int i = 0; i < num_rows; i++) {
			vector<double> datai;

			for (int j = 0; j < num_rows; j++)
				datai.push_back(0.0);
	
			_matrix.push_back(datai);
		}
		ifstream myfile;
		myfile.open(filein);
		if (myfile.is_open()) 
		{
			for (int i = 0; i < ne; i++) 
			{

				myfile >> j1>>j2;
			//edges.push_back(inc);
				_matrix[j1][j2] = 1;
				_matrix[j2][j1] = 1;
			}
			myfile.close();
		}
		else
			cout << "File not Found" << endl;
	}

	ReorderingSSM() {}
	~ReorderingSSM() {}

	// class methods

	// Function to generate degree of all the nodes
	vector<double> degreeGenerator()
	{
		vector<double> degrees;

		for (int i = 0; i < _matrix.size(); i++) {
			double count = 0;
			for (int j = 0; j < _matrix[0].size(); j++) {
				count += _matrix[i][j];
			}

			degrees.push_back(count);
		}

		return degrees;
	}

	// Implementation of Cuthill-Mckee algorithm
	vector<int> CuthillMckee()
	{
		vector<double> degrees = degreeGenerator();

		::globalDegree = degrees;

		queue<int> Q;
		vector<int> R;
		vector<pair<int, double> > notVisited;

		for (int i = 0; i < degrees.size(); i++)
			notVisited.push_back(make_pair(i, degrees[i]));

		// Vector notVisited helps in running BFS even when there are disjoind graphs
		while (notVisited.size()) {
			int minNodeIndex = 0;

			for (int i = 0; i < notVisited.size(); i++)
				if (notVisited[i].second < notVisited[minNodeIndex].second)
					minNodeIndex = i;

			Q.push(notVisited[minNodeIndex].first);

			notVisited.erase(notVisited.begin()
				+ findIndex(notVisited,
					notVisited[Q.front()].first));

			// Simple BFS
			while (!Q.empty()) {
				vector<int> toSort;

				for (int i = 0; i < _matrix[0].size(); i++) {
					if (i != Q.front() && _matrix[Q.front()][i] == 1
						&& findIndex(notVisited, i) != -1) {
						toSort.push_back(i);
						notVisited.erase(notVisited.begin()
							+ findIndex(notVisited, i));
					}
				}

				sort(toSort.begin(), toSort.end(), compareDegree);

				for (int i = 0; i < toSort.size(); i++)
					Q.push(toSort[i]);

				R.push_back(Q.front());
				Q.pop();
			}
		}

		return R;
	}

	// Implementation of reverse Cuthill-Mckee algorithm
	vector<int> ReverseCuthillMckee()
	{
		vector<int> cuthill = CuthillMckee();

		int n = cuthill.size();

		if (n % 2 == 0)
			n -= 1;

		n = n / 2;

		for (int i = 0; i <= n; i++) {
			int j = cuthill[cuthill.size() - 1 - i];
			cuthill[cuthill.size() - 1 - i] = cuthill[i];
			cuthill[i] = j;
		}

		return cuthill;
	}
};

int main()
{
	std::cout << "Hello CuthillMcKee!\n";

	int num_rows = 10,ne=14;

	vector<vector<double> > matrix;

	//for (int i = 0; i < num_rows; i++) {
	//	vector<double> datai;

	//	for (int j = 0; j < num_rows; j++)
	//		datai.push_back(0.0);

	//	matrix.push_back(datai);
	//}

	// This is the test graph,
	// check out the above graph photo
	//matrix[0] = { 0, 1, 0, 0, 0, 0, 1, 0, 1, 0 };
	//matrix[1] = { 1, 0, 0, 0, 1, 0, 1, 0, 0, 1 };
	//matrix[2] = { 0, 0, 0, 0, 1, 0, 1, 0, 0, 0 };
	//matrix[3] = { 0, 0, 0, 0, 1, 1, 0, 0, 1, 0 };
	//matrix[4] = { 0, 1, 1, 1, 0, 1, 0, 0, 0, 1 };
	//matrix[5] = { 0, 0, 0, 1, 1, 0, 0, 0, 0, 0 };
	//matrix[6] = { 1, 1, 1, 0, 0, 0, 0, 0, 0, 0 };
	//matrix[7] = { 0, 0, 0, 0, 0, 0, 0, 0, 1, 1 };
	//matrix[8] = { 1, 0, 0, 1, 0, 0, 0, 1, 0, 0 };
	//matrix[9] = { 0, 1, 0, 0, 1, 0, 0, 1, 0, 0 };

	ReorderingSSM m(matrix,num_rows,ne,"test.txt");

	vector<int> r = m.ReverseCuthillMckee();

	cout << "Permutation order of objects: " << r << endl;

	return 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started:
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file