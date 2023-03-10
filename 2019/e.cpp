#include <bits/stdc++.h>
using namespace std;
#if __cplusplus > 201703L
#include <ranges>
using namespace numbers;
#endif

struct disjoint_set{
	vector<int> p;
	disjoint_set(int n): p(n, -1){ }
	int root(int u){
		return p[u] < 0 ? u : p[u] = root(p[u]);
	}
	bool merge(int u, int v){
		u = root(u);
		v = root(v);
		if(u == v){
			return false;
		}
		if(p[u] > p[v]){
			swap(u, v);
		}
		p[u] += p[v];
		p[v] = u;
		return true;
	}
};

int main(){
	cin.tie(0)->sync_with_stdio(0);
	cin.exceptions(ios::badbit | ios::failbit);
	int n, m;
	cin >> n >> m;
	vector<array<int, 2>> edge(m);
	vector<vector<int>> adj(n);
	disjoint_set dsu(n);
	for(auto i = 0; i < m; ++ i){
		int u, v;
		cin >> u >> v, -- u, -- v;
		edge[i] = {u, v};
		dsu.merge(u, v);
		adj[u].push_back(i);
		adj[v].push_back(i);
	}
	vector<vector<int>> g(n);
	vector<int> sum(n);
	for(auto u = 0; u < n; ++ u){
		g[dsu.root(u)].push_back(u);
		sum[dsu.root(u)] += (int)adj[u].size();
	}
	vector<array<int, 2>> res;
	vector<int> origin(n), active(m), deg(n);
	for(auto u = 0; u < n; ++ u){
		deg[u] = (int)adj[u].size();
	}
	for(auto r = 0; r < n; ++ r){
		if(g[r].empty()){
			continue;
		}
		if(2 * (int)g[r].size() - 2 == sum[r]){
			for(auto u: g[r]){
				if((int)adj[u].size() == 1){
					auto e = edge[adj[u][0]];
					res.push_back({u, u ^ e[0] ^ e[1]});
				}
			}
		}
		else{
			vector<int> q;
			for(auto u: g[r]){
				if(deg[u] == 1){
					q.push_back(u);
				}
			}
			for(auto beg = 0; beg < (int)q.size(); ++ beg){
				int u = q[beg], i = -1;
				for(auto id: adj[u]){
					if(active[id]){
						continue;
					}
					i = id;
					active[id] = true;
					int v = u ^ edge[id][0] ^ edge[id][1];
					origin[u] = false;
					origin[v] = true;
					if(edge[id][0] == u){
						swap(edge[id][0], edge[id][1]);
					}
					-- deg[v];
					if(deg[v] == 1){
						q.push_back(v);
					}
				}
			}
		}
	}
	for(auto i = 0; i < m; ++ i){
		if(active[i]){
			auto [u, v] = edge[i];
			if(origin[u] && !origin[v]){
				res.push_back({u, v});
			}
		}
	}
	ranges::sort(res);
	cout << (int)res.size() << "\n";
	for(auto [u, v]: res){
		cout << u + 1 << " " << v + 1 << "\n";
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