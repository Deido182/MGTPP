#ifndef SETUP_HEADER
#define SETUP_HEADER

#include <vector>
using namespace std;

vector <int> *readTree(int n);
void removeParent(vector <int> *tree, int curr, int prev);
void rootAt(vector <int> *tree, int root);
void fillLevels(vector <int> *tree, int curr, int d, vector <int> *levels);
vector <int> *getLevels(vector <int> *tree, int root, int n);

#endif
