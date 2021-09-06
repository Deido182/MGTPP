#include <iostream>
#include <vector>
using namespace std;

vector <int> *readTree(int n) {
	vector <int> *tree = new vector <int> [n];
	for(int i = 0; i < n - 1; i ++) {
		int u, v;
		cin >> u >> v;
		tree[u].push_back(v);
		tree[v].push_back(u);
	}
	return tree;
}

void removeParent(vector <int> *tree, int curr, int prev) {
	for(int i = 0; i < tree[curr].size(); i ++) 
		if(tree[curr][i] == prev) 
			tree[curr].erase(tree[curr].begin() + i);
	for(int i = 0; i < tree[curr].size(); i ++) 
		removeParent(tree, tree[curr][i], curr);
}

void rootAt(vector <int> *tree, int root) {
	for(int i = 0; i < tree[root].size(); i ++)
		removeParent(tree, tree[root][i], root);
}

void fillLevels(vector <int> *tree, int curr, int d, vector <int> *levels) {
	levels[d].push_back(curr);
	for(int i = 0; i < tree[curr].size(); i ++)
		fillLevels(tree, tree[curr][i], d + 1, levels);
}

vector <int> *getLevels(vector <int> *tree, int root, int n) {
	vector <int> *levels = new vector <int> [n];
	fillLevels(tree, root, 0, levels);
	return levels;
}
