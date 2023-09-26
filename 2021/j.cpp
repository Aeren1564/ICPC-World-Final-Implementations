#include <bits/stdc++.h>
// #include <x86intrin.h>
using namespace std;
using namespace numbers;



int main(){
	cin.tie(0)->sync_with_stdio(0);
	cin.exceptions(ios::badbit | ios::failbit);
	int m, init, qn;
	cin >> init >> m >> qn;
	int n = 1e5;
	vector<vector<int>> adj(n), radj(n);
	for(auto i = 0; i < m; ++ i){
		char type;
		int x, y, z;
		cin >> type >> x >> y >> z, -- x, -- y, -- z;
		if(type == 'S'){
			adj[x].push_back(y);
			adj[x].push_back(z);
			radj[y].push_back(x);
			radj[z].push_back(x);
		}
		else{
			adj[x].push_back(z);
			adj[y].push_back(z);
			radj[z].push_back(x);
			radj[z].push_back(y);
		}
	}
	vector<int> order;
	{
		vector<int> indeg(n);
		int qend = 0;
		vector<int> q(n);
		for(auto u = 0; u < n; ++ u){
			indeg[u] = (int)radj[u].size();
			if(!indeg[u]){
				q[qend ++] = u;
			}
		}
		for(auto qbeg = 0; qbeg < n; ++ qbeg){
			assert(qbeg < qend);
			int u = q[qbeg];
			order.push_back(u);
			for(auto v: adj[u]){
				-- indeg[v];
				assert(indeg[v] >= 0);
				if(!indeg[v]){
					q[qend ++] = v;
				}
			}
		}
	}
	vector<int> size(n);
	size[0] = init;
	for(auto u: order){
		int deg = (int)adj[u].size();
		for(auto i = 0; i < deg; ++ i){
			size[adj[u][i]] += (size[u] + deg - 1 - i) / deg;
		}
	}
	for(auto qi = 0; qi < qn; ++ qi){
		int u, k;
		cin >> u >> k, -- u, -- k;
		while(u){
			if(size[u] <= k){
				cout << "none\n";
				goto NEXT;
			}
			if((int)radj[u].size() == 1){
				int s = radj[u][0];
				assert((int)adj[s].size() == 2);
				if(k < 2 * min(size[adj[s][0]], size[adj[s][1]])){
					k = 2 * k + (adj[s][1] == u);
				}
				else{
					k += min(size[adj[s][0]], size[adj[s][1]]);
				}
				u = s;
			}
			else{
				assert((int)radj[u].size() == 2);
				int s = radj[u][0], t = radj[u][1];
				if(k < 2 * min(size[s], size[t])){
					u = k & 1 ? t : s;
					k >>= 1;
				}
				else{
					u = size[s] < size[t] ? t : s;
					k -= min(size[s], size[t]);
				}
			}
		}
		cout << k + 1 << "\n";
		NEXT:;
	}
	return 0;
}

/*

*/

////////////////////////////////////////////////////////////////////////////////////////
//                                                                                    //
//                                   Coded by Aeren                                   //
//                                                                                    //
////////////////////////////////////////////////////////////////////////////////////////