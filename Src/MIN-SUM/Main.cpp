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

Number **S, *****Sw;
int ******F, ******H, ******P;
int **Wm, **WM;
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
	
	S = new Number*[n];
	Wm = new int*[n];
	WM = new int*[n];
	for(int i = 0; i < n; i ++) {
		S[i] = new Number[p + 1]; // initialized to +inf
		Wm[i] = new int[p + 1];
		WM[i] = new int[p + 1];
	}
	
	Sw = new Number****[eps + 1];
	F = new int*****[eps + 1];
	H = new int*****[eps + 1];
	P = new int*****[eps + 1];
	for(int i = 1; i <= eps; i ++) {
		Sw[i] = new Number***[WV.size()];
		F[i] = new int****[WV.size()];
		H[i] = new int****[WV.size()];
		P[i] = new int****[WV.size()];
		for(int j = 0; j < WV.size(); j ++) {	
			Sw[i][j] = new Number**[WV.size()];
			F[i][j] = new int***[WV.size()];
			H[i][j] = new int***[WV.size()];
			P[i][j] = new int***[WV.size()];
			for(int z = 0; z < WV.size(); z ++) {
				Sw[i][j][z] = new Number*[n];
				F[i][j][z] = new int**[n];
				H[i][j][z] = new int**[n];
				P[i][j][z] = new int**[n];
				for(int q = 0; q < n; q ++) {
					Sw[i][j][z][q] = new Number[p + 1]; // initialized to +inf
					F[i][j][z][q] = new int*[p + 1];
					H[i][j][z][q] = new int*[p + 1];
					P[i][j][z][q] = new int*[p + 1];
					for(int k = 1; k <= p; k ++) {
						F[i][j][z][q][k] = new int[T[q].size()];
						H[i][j][z][q][k] = new int[T[q].size()];
						P[i][j][z][q][k] = new int[T[q].size()];
					}
				}
			}
		}
	}
}

void leaf(int l) {
	for(int wm = 0; wm < WV.size(); wm ++) 
		for(int wM = wm; wM < WV.size(); wM ++) { // since WV is ordered
			if(w[l] < WV[wm] || w[l] > WV[wM])
				continue;
			Sw[1][wm][wM][l][1] = 0;
		}
	if(eps == 1) {
		S[l][1] = 0;
		int idx = -1;
		while(WV[++ idx] != w[l]);
		Wm[l][1] = idx;
		WM[l][1] = idx;
	}
}

void proposition2eps(int i, int wm, int wM, int epsi, int pi, int j, Number ***D) {
	int limpsj = stdVersion ? pi : min(pi, Ni[T[i][j]]);
	int bp;
	if(w[T[i][j]] < WV[wm] || w[T[i][j]] > WV[wM]) {
		for(int psj = 1; psj <= limpsj; psj ++) {
			Number sigma = S[T[i][j]][psj] + D[epsi][pi - psj][j + 1];
			if(D[epsi][pi][j] < sigma)
				continue;
			D[epsi][pi][j] = sigma;
			bp = psj;
		}
		F[epsi][wm][wM][i][pi][j] = 0;
		P[epsi][wm][wM][i][pi][j] = bp;
	} else {
		int beps, bf;
		for(int psj = 1; psj <= limpsj; psj ++) {
			Number sigma = S[T[i][j]][psj] + D[epsi][pi - psj][j + 1];
			Number eta = Number();
			int ceps = -1;
			for(int epssj = 0; epssj <= epsi - 1; epssj ++) {
				if(eta < Sw[max(1, epssj)][wm][wM][T[i][j]][psj] + D[epsi - epssj][pi - psj + 1][j + 1])
					continue;
				eta = Sw[max(1, epssj)][wm][wM][T[i][j]][psj] + D[epsi - epssj][pi - psj + 1][j + 1];
				ceps = epssj;
			}
			if(sigma < eta) {
				if(D[epsi][pi][j] < sigma)
					continue;
				D[epsi][pi][j] = sigma;
				bf = 0;
				bp = psj;
			} else {
				if(D[epsi][pi][j] < eta)
					continue;
				D[epsi][pi][j] = eta;
				bf = 1;
				beps = ceps;
				bp = psj;
			}
		}
		F[epsi][wm][wM][i][pi][j] = bf;
		H[epsi][wm][wM][i][pi][j] = beps;
		P[epsi][wm][wM][i][pi][j] = bp;
	}
}

void proposition1eps(int i, int pi) {
	for(int wm = 0; wm < WV.size(); wm ++) 
		for(int wM = wm; wM < WV.size(); wM ++) {
			if(w[i] < WV[wm] || w[i] > WV[wM])
				continue;
			if(S[i][pi] < Sw[eps][wm][wM][i][pi] + (WV[wM] - WV[wm]))
				continue;
			S[i][pi] = Sw[eps][wm][wM][i][pi] + (WV[wM] - WV[wm]);
			Wm[i][pi] = wm;
			WM[i][pi] = wM;
		}
}

void internal(int i) {
	Number ***D = new Number**[eps + 1];
	for(int epsi = 1; epsi <= eps; epsi ++) {
		D[epsi] = new Number*[p + 1];
		for(int pi = 0; pi <= p; pi ++) 
			D[epsi][pi] = new Number[T[i].size() + 1];
	}
	D[1][1][T[i].size()] = Number(0);
	for(int wm = 0; wm < WV.size(); wm ++) 
		for(int wM = wm; wM < WV.size(); wM ++) {
			if(w[i] < WV[wm] || w[i] > WV[wM])
				continue;
			for(int epsi = 1; epsi <= eps; epsi ++) 
				for(int pi = 1; pi <= p; pi ++) {
					for(int j = T[i].size() - 1; j >= 0; j --) {
						D[epsi][pi][j] = Number();
						proposition2eps(i, wm, wM, epsi, pi, j, D);
					}
					Sw[epsi][wm][wM][i][pi] = D[epsi][pi][0];
				}
		}
	for(int pi = 1; pi <= p; pi ++)
		proposition1eps(i, pi);
}

void build(int epsv, int wm, int wM, int v, int pv, vector <pair <int, int>> &Ep) {
	for(int j = 0; j < T[v].size(); j ++) {
		int bf = F[epsv][wm][wM][v][pv][j];
		int bp = P[epsv][wm][wM][v][pv][j];
		if(bf == 0) {
			int bwm = Wm[T[v][j]][bp];
			int bwM = WM[T[v][j]][bp];
			build(eps, bwm, bwM, T[v][j], bp, Ep);
		} else {
			int beps = H[epsv][wm][wM][v][pv][j];
			build(max(1, beps), wm, wM, T[v][j], bp, Ep);
			Ep.push_back({v, T[v][j]});
			epsv -= beps;
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
	vector <int> *levels = getLevels(T, R, n);
	for(int d = n - 1; d >= 0; d --) {
		for(int i = 0; i < levels[d].size(); i ++) 
			if(T[levels[d][i]].size() == 0)
				leaf(levels[d][i]);
			else
				internal(levels[d][i]);
	}
	if(S[R][p].inf) {
		cout << "The problem is unfeasible\n";
		return;
	}
	int bwm = Wm[R][p];
	int bwM = WM[R][p];
	vector <pair <int, int>> Ep;
	build(eps, bwm, bwM, R, p, Ep);
	int *CC = connectedComponents(Ep);
	cout << S[R][p].value << "\n";
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
