// ReverseCuthillMcKee.cpp : This file contains the 'main' function. Program execution begins and
// ends there. C++ program for Implementation of Reverse Cuthill Mckee Algorithm
#include <iostream>
#include <stdio.h>
#include <fstream>
#include <vector>
#include <queue>
#include <string>

using namespace std;
vector<int> globalDegree;
int ne = 0;
vector<vector<int>> edges;
int findIndex(vector<pair<int, int> > a, int x)
{
	for (int i = 0; i < a.size(); i++)
		if (a[i].first == x)
			return i;
	return -1;
}

int findIndex2(vector<pair<int, int> > a, int x)
{
	for (int i = 0; i < a.size(); i++)
		if (a[i].second == x)
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

template <typename T>
ostream& operator<<(ostream& out, vector<vector<T>> const& v)
{
	for (int i = 0; i < v.size(); i++)
		out << v[i][0] << ' '<<v[i][1]<<endl;
	return out;
}

class ReorderingSSM 
{
	private:
		vector<vector<int> > _matrix;
		int index1=-1,index2=-1;

	public:
	// Constructor and Destructor
	ReorderingSSM(vector<vector<int>> edges,vector<pair<int,int>> nodes,int nn)
	{

		//_matrix = m;
		int j1, j2;
		//int ne1 = edges.size();
		for (int i = 0; i < nn; i++) {
			vector<int> datai;

			for (int j = 0; j < nn; j++)
				datai.push_back(0.0);
			_matrix.push_back(datai);
		}
		for (int i = 0; i < edges.size(); i++) {
			j1 = edges[i][0];
			j2 = edges[i][1];
			index1 = findIndex2(nodes, j1);
			index2 = findIndex2(nodes, j2);
			_matrix[index1][index2] = 1;
			_matrix[index2][index1] = 1;
		}

	}

	ReorderingSSM() {}
	~ReorderingSSM() {}

	// class methods

	// Function to generate degree of all the nodes
	vector<int> degreeGenerator()
	{
		vector<int> degrees;

		for (int i = 0; i < _matrix.size(); i++) {
			int count = 0;
			for (int j = 0; j < _matrix[0].size(); j++) {
				count += _matrix[i][j];
			}

			degrees.push_back(count);
		}
		//cout << degrees << endl;
		return degrees;
	}

	// Implementation of Cuthill-Mckee algorithm
	vector<int> CuthillMckee()
	{
		vector<int> degrees = degreeGenerator();

		::globalDegree = degrees;

		queue<int> Q;
		vector<int> R;
		vector<pair<int, int> > notVisited;

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
	vector<pair<int, int>> nodes;
	int diff = 0;
	int band = 0;
	string filein;
	int ne = 0,nn=0;
	int j1 = 0, j2 = 0, index1=0,index2=0;
	std::cout << "Enter File Name!\n";
	cin >> filein ;
	ifstream myfile;
	myfile.open(filein);
	if (myfile.is_open())
	{
		diff = 0;
		myfile >> nn >> ne;
		for (int i = 0; i < nn; i++) {
			myfile >> j1;
			nodes.push_back(make_pair(i,j1));
		}
		for(int i=0;i<ne;i++)
		{

			myfile >> j1 >> j2;
			band = abs(j1 - j2);

			if (band > diff) diff = band;
			vector<int> inc;
			inc.push_back(j1);
			inc.push_back(j2);
			edges.push_back(inc);
			//ne++;

		}

		myfile.close();
		cout << "Number of edges: "<<ne<<' ' <<"Number of Node: " <<' ' << nn<<" Bandwidth:  " << diff << endl;
	}
	else
	{
		cout << "File not Found" << endl;
		return -1;
	}

	// This is the test graph,
	// check out the above graph photo
	//matrix[0] = { 0, 1, 0, 0, 0, 0, 1, 0, 1, 0 };
	//matrix[1] = { 1, 0, 0, 0, 1, 0, 1, 0, 0, 1 };
	//matrix[2] = { 0, 0, 0, 0, 1, 0, 1, 0, 0, 0 };
	//matrix[3] = { 0, 0, 0, 0, 1, 1, 0, 0, 1, 0 };
	//matrix[4] = { 0, 0, 0, 0, 1, 1, 0, 0, 1, 0 };
	//matrix[5] = { 0, 0, 0, 1, 1, 0, 0, 0, 0, 0 };
	//matrix[6] = { 1, 1, 1, 0, 0, 0, 0, 0, 0, 0 };
	//matrix[7] = { 0, 0, 0, 0, 0, 0, 0, 0, 1, 1 };
	//matrix[8] = { 1, 0, 0, 1, 0, 0, 0, 1, 0, 0 };
	//matrix[9] = { 0, 1, 0, 0, 1, 0, 0, 1, 0, 0 };

	ReorderingSSM m(edges,nodes,nn);

	vector<int> r = m.ReverseCuthillMckee();
	int r0; int r1;
	cout << "Permutation order of objects: " << r << endl;
	cout << "Original Edges" << endl;
	cout << edges;
	cout << "New Edges" << endl;

	diff = 0;
	for (int i = 0; i < edges.size(); i++) {

		j1 = edges[i][0];
		j2 = edges[i][1];
		index1 = findIndex2(nodes, j1);
		index2 = findIndex2(nodes, j2);
		r0 = r[index1]; r1 = r[index2];
		band = abs(r0 - r1);
		if (band>diff) diff = band;
		cout << nodes[r0].second << ' ' << nodes[r1].second << endl;
	}
	cout << "New bandwidth: " << diff << endl;
	return 0;
}

