#include "Utils/Number.h"
#include "../CommonUtils/SetupUtils.h"
#include <iostream>
#include <algorithm>
#include <vector>
#include <set>
#include <list>
#include <assert.h>
#include <string>

using namespace std;

bool **B;
Number ***Hw;
int **Wm, ****F, ****P;
vector <Number> WV;

int *Ni;
bool stdVersion;

const int R = 0;

int n, p, eps;
Number *w;
vector <int> *T;

void fill(int curr) {
	Ni[curr] = 1;
	for(int next : T[curr]) {
		fill(next);
		Ni[curr] += Ni[next];
	}
}

void setup() {
	cin >> n;
	w = new Number[n];
	for(int i = 0; i < n; i ++) {
		ll wi;
		cin >> wi;
		w[i] = Number(wi);
	}
	T = readTree(n);
	rootAt(T, R);
	cin >> p >> eps;
	
	set <ll> s; // it is a tree set, so WV is ordered
	for(int i = 0; i < n; i ++)
		s.insert(w[i].value);
	for(auto value : s)
		WV.push_back(Number(value));
	
	if(!stdVersion) {
		Ni = new int[n];
		for(int i = 0; i < n; i ++)
			Ni[i] = 0;
		fill(R);
	}
	
	B = new bool*[n];
	Wm = new int*[n];
	for(int i = 0; i < n; i ++) {
		B[i] = new bool[p + 1];
		Wm[i] = new int[p + 1];
	}
	
	Hw = new Number**[WV.size()];
	F = new int***[WV.size()];
	P = new int***[WV.size()];
	for(int i = 0; i < WV.size(); i ++) {
		Hw[i] = new Number*[n];
		F[i] = new int**[n];
		P[i] = new int**[n];
		for(int j = 0; j < n; j ++) {	
			Hw[i][j] = new Number[p + 1];
			F[i][j] = new int*[p + 1];
			P[i][j] = new int*[p + 1];
			for(int z = 1; z <= p; z ++) {
				F[i][j][z] = new int[T[j].size()];
				P[i][j][z] = new int[T[j].size()];
			}
		}
	}
}

void leaf(int l, Number gamma) {
	for(int wm = 0; wm < WV.size(); wm ++) 
		if(WV[wm] <= w[l] && w[l] <= WV[wm] + gamma) 
			Hw[wm][l][1] = Number(1);
	if(eps == 1) {
		B[l][1] = true;
		int idx = -1;
		while(WV[++ idx] != w[l]);
		Wm[l][1] = idx;
	}
}

void proposition2eps(int i, int wm, Number gamma, int pi, int j, Number **D) {
	int limpsj = stdVersion ? pi : min(pi, Ni[T[i][j]]);
	int bp;
	if(w[T[i][j]] < WV[wm] || w[T[i][j]] > WV[wm] + gamma) {
		for(int psj = 1; psj <= limpsj; psj ++) {
			Number sigma = !B[T[i][j]][psj] ? Number() : D[pi - psj][j + 1];
			if(D[pi][j] > sigma)
				continue;
			D[pi][j] = sigma;
			bp = psj;
		}
		F[wm][i][pi][j] = 0;
		P[wm][i][pi][j] = bp;
	} else {
		int bf;
		for(int psj = 1; psj <= limpsj; psj ++) {
			Number sigma = !B[T[i][j]][psj] ? Number() : D[pi - psj][j + 1];
			Number eta = Hw[wm][T[i][j]][psj] + D[pi - psj + 1][j + 1];
			if(sigma > eta) {
				if(D[pi][j] > sigma)
					continue;
				D[pi][j] = sigma;
				bf = 0;
				bp = psj;
			} else {
				if(D[pi][j] > eta)
					continue;
				D[pi][j] = eta;
				bf = 1;
				bp = psj;
			}
		}
		F[wm][i][pi][j] = bf;
		P[wm][i][pi][j] = bp;
	}
}

void proposition1eps(int i, int pi, Number gamma) {
	for(int wm = 0; wm < WV.size(); wm ++) {
		if(w[i] < WV[wm] || w[i] > WV[wm] + gamma)
			continue;
		if(Hw[wm][i][pi].minus_inf || Hw[wm][i][pi].value < eps)
			continue;
		B[i][pi] = true;
		Wm[i][pi] = wm;
		break;
	}
}

void internal(int i, Number gamma) {
	Number **D = new Number*[p + 1];
	for(int pi = 0; pi <= p; pi ++) 
		D[pi] = new Number[T[i].size() + 1];
	D[1][T[i].size()] = Number(1);
	for(int wm = 0; wm < WV.size(); wm ++) {
		if(w[i] < WV[wm] || w[i] > WV[wm] + gamma)
			continue;
		for(int pi = 1; pi <= p; pi ++) {
			for(int j = T[i].size() - 1; j >= 0; j --) {
				D[pi][j] = Number();
				proposition2eps(i, wm, gamma, pi, j, D);
			}
			Hw[wm][i][pi] = D[pi][0];
		}
	}
	for(int pi = 1; pi <= p; pi ++)
		proposition1eps(i, pi, gamma);
}

bool check(Number gamma) {
	for(int i = 0; i < n; i ++)
		for(int j = 1; j <= p; j ++)
			B[i][j] = false;
	for(int z = 0; z < WV.size(); z ++)
		for(int i = 0; i < n; i ++)
			for(int j = 1; j <= p; j ++)
				Hw[z][i][j] = Number();
	
	vector <int> *levels = getLevels(T, R, n);
	for(int d = n - 1; d >= 0; d --) {
		for(int i = 0; i < levels[d].size(); i ++) 
			if(T[levels[d][i]].size() == 0)
				leaf(levels[d][i], gamma);
			else
				internal(levels[d][i], gamma);
	}
	return B[R][p];
}


void build(int wm, int v, int pv, vector <pair <int, int>> &Ep) {
	for(int j = 0; j < T[v].size(); j ++) {
		int bf = F[wm][v][pv][j];
		int bp = P[wm][v][pv][j];
		if(bf == 0) {
			int bwm = Wm[T[v][j]][bp];
			build(bwm, T[v][j], bp, Ep);
		} else {
			build(wm, T[v][j], bp, Ep);
			Ep.push_back({v, T[v][j]});
		}
		pv -= bp - bf;
	}
}

void dfs(int curr, vector <int> *G, int *CC, const int C) {
	CC[curr] = C;
	for(int next : G[curr])
		if(CC[next] != C)
			dfs(next, G, CC, C);
}

int *connectedComponents(vector <pair <int, int>> &Ep) {
	vector <int> *G = new vector <int> [n];
	for(int i = 0; i < Ep.size(); i ++) {
		G[Ep[i].first].push_back(Ep[i].second);
		G[Ep[i].second].push_back(Ep[i].first);
	}
	const int NONE = 0;
	int *CC = new int[n];
	for(int i = 0; i < n; i ++)
		CC[i] = NONE;
	int C = 1;
	for(int i = 0; i < n; i ++) {
		if(CC[i] != NONE)
			continue;
		dfs(i, G, CC, C ++);
	}
	return CC;
}

void solve() {
	if(eps > n / p) {
		cout << "The problem is unfeasible\n";
		return;
	}
	vector <Number> Gamma;
	for(int wm = 0; wm < WV.size(); wm ++)
		for(int wM = wm; wM < WV.size(); wM ++)
			Gamma.push_back(WV[wM] - WV[wm]);
	sort(Gamma.begin(), Gamma.end());
	int zmin = 0, zmax = Gamma.size() - 1, bz = -1;
	while(zmin <= zmax) {
		int z = (zmin + zmax) >> 1;
		if(check(Gamma[z])) {
			bz = z;
			zmax = z - 1;
		} else 
			zmin = z + 1;
	}
	if(bz == -1) {
		cout << "The problem is unfeasible\n";
		return;
	}
	check(Gamma[bz]);
	int bwm = Wm[R][p];
	vector <pair <int, int>> Ep;
	build(bwm, R, p, Ep);
	int *CC = connectedComponents(Ep);
	cout << Gamma[bz].value << "\n";
	for(int i = 0; i < n; i ++)
		cout << CC[i] << " ";
	cout << "\n";
}

int main(int argc, char *argv[]) {
	string opt1 = "-std", opt2 = "-diam";
	if(argc != 2) {
		cout << "Error\n";
		return 0;
	} else if(opt1.compare(argv[1]) == 0)
		stdVersion = true;
	else if(opt2.compare(argv[1]) == 0)
		stdVersion = false;
	else {
		cout << "Error\n";
		return 0;
	}
	setup();
	solve();
	return 0;
}
