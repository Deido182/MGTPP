#include "../Utils/Number.h"
#include "../Utils/SetupUtils.h"
#include <iostream>
#include <algorithm>
#include <vector>
#include <set>
#include <map>

using namespace std;

const int INF = 1 << 16;
const int ROOT = 0;

vector <int> *tree;
Number *w;
int n, p, eps;

void evaluate(set <int> &chosen, map <int, pair <int, int>> &minmax, map <int, int> &cnt, int curr, int leader) {
	if(chosen.count(curr) != 0)
		leader = curr;
	minmax.at(leader).first = min(minmax.at(leader).first, (int)w[curr].value);
	minmax.at(leader).second = max(minmax.at(leader).second, (int)w[curr].value);
	cnt.at(leader) ++;
	for(int i = 0; i < tree[curr].size(); i ++) 
		evaluate(chosen, minmax, cnt, tree[curr][i], leader);
}

int tryAll(int toChoose, int total, set <int> &chosen, int from) {
	int ans = INF;
	if(chosen.size() == toChoose) {
		map <int, pair <int, int>> minmax = map <int, pair <int, int>> ();
		for(auto& c : chosen)
			minmax.insert({c, {INF, -INF}});
		map <int, int> cnt = map <int, int> ();
		for(auto& c : chosen)
			cnt.insert({c, 0});
		evaluate(chosen, minmax, cnt, ROOT, ROOT);
		bool ok = true;
		for(auto& c : chosen)
			ok &= cnt.at(c) >= eps;
		if(ok) {
			int sum = 0;
			for(auto& e : minmax)
				sum += e.second.second - e.second.first;
			ans = sum;
		}
	} else {
		for(int i = from; i < total; i ++) {
			chosen.insert(i);
			ans = min(ans, tryAll(toChoose, total, chosen, i + 1));
			chosen.erase(i);
			ans = min(ans, tryAll(toChoose, total, chosen, i + 1));
		}
	}
	return ans;
}

int trivialSolver() {
	set <int> chosen = set <int> ();
	chosen.insert(ROOT);
	return tryAll(p, n, chosen, 1);
}

int main() {
	cin >> n;
	w = readWeights(n);
	tree = readTree(n);
	cin >> p >> eps;
	rootAt(tree, ROOT);
	int ans = trivialSolver();
	if(ans != INF)
		cout << ans << "\n";
	else
		cout << "The problem is unfeasible\n";
}
